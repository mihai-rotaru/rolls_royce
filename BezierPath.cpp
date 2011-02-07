#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#include "BezierPath.h"
#include <GL/glu.h>

#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/regex.hpp>

namespace xmx {

void xmx::BezierPath::addCurve( 
        GLfloat e1_x, GLfloat e1_y,
        GLfloat c1_x, GLfloat c1_y,
        GLfloat c2_x, GLfloat c2_y,
        GLfloat e2_x, GLfloat e2_y )
{
    boost::shared_ptr<BezierCurve> mptr
        ( new BezierCurve( e1_x, e1_y, c1_x, c1_y, c2_x, c2_y, e2_x, e2_y ));

    curves.push_front( mptr );
}


void xmx::BezierPath::draw()
{
    BOOST_FOREACH( boost::shared_ptr<BezierCurve> curve_ptr, curves )
    {
        curve_ptr->draw();
    }
}


void xmx::BezierPath::print()
{
    cout<<"BezierPath @ "<<this<<endl;
    BOOST_FOREACH( boost::shared_ptr<BezierCurve> curve_ptr, curves )
    {
        curve_ptr->draw();
    }
}

// works with files exported from Inkscape as "PovRay ( paths and shapes only)"
// loads the first 'bezier_spline' found
void xmx::BezierPath::loadFromPovFile( char* filename )
{
    string line;
    int line_no =             0; // current line number ( first line is 1 )
    int nr_points =           0; // number of total expected points
    int bezier_spline_line = -1; // on which line is 'bezier_spline'
    int nr_points_line =     -1; // on which line is 'nr points'
    int next_point =         -1; // next expected point number

    cout<<"loading file '" << filename <<"'..."<<endl;
    ifstream my_file( filename, ios::in );
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
//                cout<<"//nr points @ line "<< line_no <<endl;
                static const boost::regex e( "(\\d{1,}) //nr points$", boost::regex::extended );
                boost::smatch what;
                if( regex_search( line, what, e ))
                {
                    nr_points = boost::lexical_cast<int>( what[1] );
                    cout<<"number of expected points: "<< nr_points << endl;
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
                    cout<<endl<<"Curve # "<< point_in_line << " on line "<< line_no <<": "<< endl;

                    // this line contains coords for 4 points - a Bezier curve
                    Point E1, C1, C2, E2;

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
                        cout<<"x: "<<boost::lexical_cast<GLfloat>( result2[0] )<< "; ";
                        cout<<"y: "<<boost::lexical_cast<GLfloat>( result2[1] )<< "; ";

                        cout<<endl;
                    }

                }
            }
        
        }
        my_file.close();
    }
    else cout<<"Unable to open file: '" << filename << "'" << endl;
}


void xmx::BezierPath::resizeTo( int newWidth )
{
    return /* something */;
}


xmx::BezierPath::BezierPath()
{
}


xmx::BezierPath::~BezierPath()
{
}

} // namespace xmx
