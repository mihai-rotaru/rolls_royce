#include <fstream>
#include <iostream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <stdexcept>
using namespace std;

#include <GL/glu.h>
#include <GL/glut.h>

#include <boost/shared_ptr.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/lexical_cast.hpp>

#include "globals.h"
#include "utils.h"
#include "Point.h"
#include "BezierCurve.h"
#include "Shape.h"
using namespace xmx;
typedef boost::shared_ptr<Shape> sptrShape;


// if `str` matches `myRegex`, try parsing the first match into an integer
// NOTE: lexical_cast is said to be slow 
// [http://stackoverflow.com/questions/1250795/very-poor-boostlexical-cast-performance]
    template < typename T >
T getFromRegex( string haystack, const boost::regex& myRegex )
{
    boost::smatch match;
    if( regex_search( haystack, match, myRegex ))
        return boost::lexical_cast<T>( match[1] );
    else
        throw( "exception in getFromRegex( string, regex )");
}

// assumes `line` is a string containing the coordinates 4 points, something like:
// /*   1*/ <140.00000000, 440.00000000>, <210.00000000, 370.00000000>, <490.00000000, 560.00000000>, <400.00000000, 410.00000000>,
// this function parses lines of this type, returning a shared_ptr to the Bezier curved described by the points
// specified on the line.
boost::shared_ptr< BezierCurve > parseBezierCurve( string line )
{
    cout << "parsing line " << line <<" for a Bezier curve" << endl;

    static const boost::regex split_coords_regex ( ">, <|/ <|>,$|>$", boost::regex::extended );
    static const boost::regex curve_no_regex     ( "\\s*/\\*\\s*(\\d{1,})\\*/ <", boost::regex::extended );
    static const boost::regex comma_split_regex  ( ", ", boost::regex::extended );

    Point pointz[4];

    vector< string > result;

    boost::algorithm::split_regex( result, line, split_coords_regex );

    for( int i=1; i< result.size()-1; i++ )
    {   // split the pairs into numbers
        vector<string> result2;
        boost::algorithm::split_regex ( result2, result[i], comma_split_regex );

        pointz[i-1].x = boost::lexical_cast<GLfloat>( result2[0] );
        pointz[i-1].y = boost::lexical_cast<GLfloat>( result2[1] );

        if( DEBUG_POV_LOADING ) cout<<"P"<< i-1 <<": x: "<< pointz[i-1].x <<", y: "<< pointz[i-1].y <<endl;
    }

    boost::shared_ptr< BezierCurve > mbc( new BezierCurve( pointz[0], pointz[1], pointz[2], pointz[3] ));
    return mbc;
}

// try parsing a Shape from `lines`, starting with `start_line`
// and if parsing is successufll, put the last parsed line into `end_line`
boost::shared_ptr< Shape > parseShape( vector< string >& lines, int& line_no )
{
    cout<< "parsing Shape starting with line " << line_no << endl;
    string line = lines[ line_no ];

    static const boost::regex shape_end_regex    ( "#{3} end (path[-\\d]{4,})", boost::regex::extended );
    static const boost::regex curve_regex        ( "\\s*/\\*\\s*(\\d{1,})\\*/ <", boost::regex::extended );
    static const boost::regex shape_name_regex   ( "\\s*#declare (path[-\\d]{4,})\\s*=\\s*prism", boost::regex::extended );

    string shape_name = getFromRegex< string >( line, shape_name_regex );
    cout<< "shape name: "<< shape_name << endl;
    boost::shared_ptr< Shape > sptr_shape( new Shape());
    sptr_shape->name = shape_name;
    sptr_shape->isBezier = true;

    bool shape_finished = false;

    while( line_no <= lines.size() && !shape_finished )
    {
        if( regex_search( line, curve_regex ))
            sptr_shape->addBezierCurve( parseBezierCurve( line ));

        else if( regex_search( line, shape_end_regex ) && ( getFromRegex< string >( line, shape_end_regex ) == sptr_shape->name ))
        {
            cout<<"sptr_shape " << sptr_shape->name << " end at: " << line_no << endl;
            shape_finished = true;
        }
        else
            cout<<"no bezier, no end: " << line << endl;

        line_no ++;
        line = lines[ line_no ];
    }

    if( !shape_finished )
    {
        string msg = "Pov parsing error: sptr_shape " + sptr_shape->name + " not finished";
        throw runtime_error( msg );
    }

    return sptr_shape;

}

void loadPovFile( string filename, list< sptrShape >& shape_list )
{

    //    list< sptr_shape > mlst = (list< sptr_shape >*)*shape_list;
    cout<< "loading " << filename << " into " << &shape_list << endl;
    int total_lines = 0;
    vector<string> lines;
    lines.push_back( filename );

    // read the whole file in memory
    ifstream my_file;
    my_file.open( filename.c_str(), ifstream::in );
    string line;
    while( getline( my_file, line ))
    {
        total_lines++;
        lines.push_back( line );
    }

    cout<< "total lines: " << total_lines << endl;

    // process the file
    int header_start               = 0;     // the line number where the header starts
    int num_shapes                 = 0;
    int num_segments               = 0;
    int num_nodes                  = 0;             

    // find the header
    int cline = 1; // the current line
    while( !header_start && cline <= total_lines )
    {
        cline++;
        if ( lines[ cline ].find( "##   Exports in this file" ) != -1 ) 
            header_start = cline;
    }

    // no header ?
    if( !header_start && STRICT_POV )
        throw runtime_error("Pov parsing error: can't find the header");

    cout << "header start: " << header_start << endl;

    // setup some regexes
    static const boost::regex shapes_regex       ( "^##\\s{1,}Shapes\\s{1,}: (\\d{1,})$", boost::regex::extended );
    static const boost::regex segments_regex     ( "^##\\s{1,}Segments\\s{1,}: (\\d{1,})$", boost::regex::extended );
    static const boost::regex nodes_regex        ( "^##\\s{1,}Nodes\\s{1,}: (\\d{1,})$", boost::regex::extended );
    static const boost::regex curve_no_regex     ( "\\s*/\\*\\s*(\\d{1,})\\*/ <", boost::regex::extended );

    // parse the header
    if( STRICT_POV )
    {
        if( lines[ header_start + 2 ].find( "Shapes" ) == -1 )
        {
            cout<<"line: "<< header_start + 2 << endl;
            throw runtime_error("Pov parsing error: expected 'Shapes'; line: '" + lines[ header_start + 2 ] + "'" );
        }
        else
        {
            cout<<"getting asdasd"<< endl;
            num_shapes = getFromRegex<int>( lines[ header_start + 2 ], shapes_regex );
        }
    }

    if( STRICT_POV )
    {
        if( lines[ header_start + 3 ].find( "Segments" ) == -1 )
        {
            cout<<"line: "<< header_start + 3 << endl;
            throw runtime_error("Pov parsing error: expected 'Segments'; line: '" + lines[ header_start + 2 ]  + "'" );
        }
        else
            num_segments = getFromRegex<int>( lines[ header_start + 3 ] , segments_regex );
    }

    if( STRICT_POV )
    {
        if( lines[ header_start + 4 ].find( "Nodes" ) == -1 )
        {
            cout<<"line: "<< header_start + 4 << endl;
            throw runtime_error("Pov parsing error: expected 'Nodes'; line: '" + lines[ header_start + 2 ] + "'" );
        }
        else
            num_nodes = getFromRegex<int>( lines[ header_start + 4 ], nodes_regex );
    }

    // we're done with the header;
    // now let's look for those shapes

    // we'll need some regex'es
    static const boost::regex shape_start        ( "\\s*#declare (path[-\\d]{4,})\\s*=\\s*prism", boost::regex::extended );

    bool inside_shape = false;

    // iterate over the rest of the lines
    cline = header_start + 5;                                                                                         
    while( cline <= total_lines )
    {
        cout<< "processing line: " << cline << " - '" << lines[ cline ] << "'" << endl;
        string line = lines[ cline ];

        // does the line contain a curve ?
        if ( regex_search( line, curve_no_regex ))
        {
            if( !inside_shape )
            {
                cout<<"line: "<< cline << endl;
                throw runtime_error( "Pov parsing error: points encoutered outside a shape definition" );
            }
        }

        // does the line define the start of a shape ?
        else if ( regex_search( line, shape_start ))
            if( inside_shape )
            {

                cout<<"line: "<< cline << endl;
                cout<<"Pov parsing warning: nested shapes not supported ( line " << cline + 1 << endl;
                inside_shape = true;
            }
            else 
            {
                inside_shape = true;

                shape_list.push_back( parseShape( lines, cline ) );
                cout << "parsed shape; " << cline << endl;
                inside_shape = false;
            }

        cline ++;
    }

    cout<< "processing of " << filename << " complete" << endl;
}

void printText( int nX, int nY, string text )
{
    int lines;
    const char *pszText = text.c_str();
    const char *p;

    glColor3ub( 150, 150, 150 );
    glRasterPos2i( nX, nY );

    for( p=pszText, lines=0; *p; p++ )
    {
        if( *p == '\n' )
        {
            lines++;
            glRasterPos2i( nX, nY-(lines*18) );
        }

        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12,  *p );
    }
}

void print3DVertex( GLint size, GLint* count, GLfloat* buff )
{
    // if the DEBUG_FEEDBACK_TOKENS flag is not set, return
    if( DEBUG_FEEDBACK_TOKENS )
        cout << "printing token from buffer at " << buff << ", count = " << *count << endl;
    else return;

    // 3D vertices, no color infor, so should be 3 floats
    for( int i=0; i<3; i++ )
    {
        cout<< "index: " << i << " ; ( " << size - (*count) << " )   ";
        cout<< "value: " << buff[ size-(*count )];
        *count = *count - 1;
        cout << endl;
    }
}

void printFeedbackBuffer( GLfloat* buffer, GLint size )
{
    cout << "-----------------------------------------------------" << endl;
    cout << " FeedbackBuffer at: " << buffer << " ;  size = " << size << endl;
    cout << "-----------------------------------------------------" << endl;

    GLint count;
    GLfloat token;
    count = size;
    while (count>=0) 
    {
        token = buffer[size-count];
        cout<< endl << "at token " << size-count << endl;
        count--;
        
        if (token == GL_PASS_THROUGH_TOKEN) 
        {
            cout<< "GL_PASS_THROUGH_TOKEN" << endl;
            cout<< buffer[size-count] << endl;
            count--;
        }
        
        else if (token == GL_POINT_TOKEN)
        {
            cout<< "GL_POINT_TOKEN" << endl;
            print3DVertex(size, &count, buffer);
        }
        
        else if (token == GL_LINE_TOKEN)
        {
            cout<< "GL_LINE_TOKEN" << endl;
            print3DVertex(size, &count, buffer);
            print3DVertex(size, &count, buffer);
        }
        
        else if (token == GL_LINE_RESET_TOKEN)
        {
            cout<< "GL_LINE_RESET_TOKEN" << endl;
            print3DVertex(size, &count, buffer);
            print3DVertex(size, &count, buffer);
        }
        else
        {
            cout << "UNIDENTIFIED TOKEN; count = "<< count << endl;
        }
    }

}
