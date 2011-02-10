#include <iostream>
#include <fstream>
using namespace std;

#include <GL/glu.h>

#include <boost/shared_ptr.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/foreach.hpp>

#include "globals.h"
#include "Point.h"
#include "Primitive.h"
#include "Shape.h"
#include "BezierCurve.h"
#include "Line.h"

namespace xmx {

Shape::Shape()
{
    if( DEBUG_CTOR_DTOR ) cout<<"ctor for Shape @ " << this << endl;
}

Shape::~Shape()
{
    if( DEBUG_CTOR_DTOR ) cout<<"dtor for Shape @ " << this << endl;
}

void Shape::addBezierCurve(
        GLfloat e1_x, GLfloat e1_y,
        GLfloat c1_x, GLfloat c1_y,
        GLfloat c2_x, GLfloat c2_y,
        GLfloat e2_x, GLfloat e2_y )
{
    boost::shared_ptr<BezierCurve> mptr
        ( new BezierCurve( e1_x, e1_y, c1_x, c1_y, c2_x, c2_y, e2_x, e2_y ));

    primitives.push_front( mptr );
}

void Shape::addBezierCurve(
        Point& P0, Point& P1, Point& P2, Point& P3
        )
{
    boost::shared_ptr<BezierCurve> mptr
        ( new BezierCurve( P0, P1, P2, P3 ));

    primitives.push_front( mptr );
}

void Shape::addLine( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 )
{
    boost::shared_ptr< Line > mptr
        ( new Line( x1, y1, x2, y2 ));

    primitives.push_front( mptr );
}


void Shape::scale( GLfloat x_dir, GLfloat y_dir )
{
    return;
}

void Shape::rotate( GLfloat )
{
    return /* something */;
}

void Shape::rotate( GLfloat, GLfloat, GLfloat )
{
    return /* something */;
}

void Shape::move( GLfloat x_dir, GLfloat y_dir )
{
    if ( DEBUG_TRANSF_MOVE ) cout << "moving Shape @ " << this << endl;
    BOOST_FOREACH( boost::shared_ptr< Primitive > pPrimitive, primitives )
    {
        pPrimitive->move( x_dir, y_dir );
    }
}

void Shape::draw()
{
    BOOST_FOREACH( boost::shared_ptr< Primitive > pPrimitive, primitives )
    {
        pPrimitive->draw();
    }
}

GLfloat Shape::distanceTo( GLfloat x, GLfloat y )
{
    return -1001;
}

Shape loadFromPovFile( string  filename )
{
    Shape shape;

    string line;
    int line_no =             0; // current line number ( first line is 1 )
    int nr_points =           0; // number of total expected points
    int bezier_spline_line = -1; // on which line is 'bezier_spline'
    int nr_points_line =     -1; // on which line is 'nr points'
    int next_point =         -1; // next expected point number
    int segments =           -1; // total number of segments in this file
    int total_shapes =       -1; // total number of shapes in this file

    cout<<"loading file '" << filename <<"'..."<<endl;
    ifstream my_file;
    my_file.open( filename.c_str(), ifstream::in );
    if ( my_file.is_open())
    {
        while( my_file.good() )
        {
            getline( my_file, line );
            line_no++;
            if( line.find("bezier_spline") != -1 )
            {
//                cout<<"bezier_spline on line "<< line_no <<endl;
                bezier_spline_line = line_no;
            }

            else if( line.find("//nr points") != -1 && bezier_spline_line != -1 )
            {
                static const boost::regex e( "(\\d{1,}) //nr points$", boost::regex::extended );
                boost::smatch what;
                if( regex_search( line, what, e ))
                {
                    nr_points = boost::lexical_cast<int>( what[1] );
                    if( DEBUG_POV_LOADING ) cout<<"number of expected points: "<< nr_points << endl;
                }
            }
            else if( line.find("Segments : ") != -1 )
            {
                static const boost::regex seg( "^##\\s{1,}Segments : (\\d{1,})$", boost::regex::extended );
                boost::smatch seg_match;
                if( regex_search( line, seg_match, seg ))
                {
                    segments = boost::lexical_cast<int>( seg_match[1] );
                    if( DEBUG_POV_LOADING ) cout<<"number of segments: "<< segments << endl;
                }
            }
            else if( line.find("Shapes") != -1 )
            {
                static const boost::regex shapes_reg( "^##\\s{1,}Shapes\\s{1,}: (\\d{1,})$", boost::regex::extended );
                boost::smatch shapes_match;
                if( regex_search( line, shapes_match, shapes_reg ))
                {
                    total_shapes = boost::lexical_cast<int>( shapes_match[1] );
                    if( DEBUG_POV_LOADING ) cout<<"number of shapes: "<< total_shapes << endl;
                }
            }
            else
            {
//                cout<<"line " << line_no <<" may be a point defining line" << endl;
                static const boost::regex pt( "\\s*/\\*\\s*(\\d{1,})\\*/ <", boost::regex::extended );
                boost::smatch what2;
                if( regex_search( line, what2, pt ))
                {   // split the line into pairs
                    int point_in_line = boost::lexical_cast<int>( what2[1] );
                    if( DEBUG_POV_LOADING ) cout<<endl<<"Curve # "<< point_in_line << " on line "<< line_no <<": "<< endl;

                    // this line contains coords for 4 points - a Bezier curve
                    Point pointz[4];

                    vector<string> result;
                    boost::algorithm::split_regex (
                         result,
                         line,
                         boost::regex( ">, <|/ <|>,$|>$" )
                            );
                    
                    for( int i=1; i< result.size()-1; i++ )
                    {   // split the pairs into numbers
                        vector<string> result2;
                        boost::algorithm::split_regex (
                             result2,
                             result[i],
                             boost::regex( ", " )
                                );
                        pointz[i-1].x = boost::lexical_cast<GLfloat>( result2[0] );
                        pointz[i-1].y = boost::lexical_cast<GLfloat>( result2[1] );
                        if( DEBUG_POV_LOADING ) cout<<"P"<< i-1 <<": x: "<< pointz[i-1].x <<", y: "<< pointz[i-1].y <<endl;
                                         
                    }

                    // we should have the 4 points in 'pointz'
                    // so we add the Bezier formed by these points to our Shape
                    shape.addBezierCurve( pointz[0], pointz[1], pointz[2], pointz[3] );
                }
            }
        
        }
        cout<<"processing of " << filename << " complete." << endl;
        my_file.close();
    }
    else cout<<"Unable to open file: '" << filename << "'" << endl;
    return shape;
}

} // namespace xmx
