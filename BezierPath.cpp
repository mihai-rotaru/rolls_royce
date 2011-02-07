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
                {
                    int point_in_line = boost::lexical_cast<int>( what2[1] );
                    cout<<"points on line "<< line_no <<": "<< point_in_line << endl;

                    // this line contains coords for 4 points - a Bezier curve
                    Point E1, C1, C2, E2;

//                    size_t start = line.find('<');
//                    size_t end = line.find('\n');
//                    cout<<"start: "<<start<<", end: "<<end<<endl;

                    vector<string> result;
                    boost::algorithm::split_regex (
                         result,
                         line,
                         boost::regex( ">, <|/ <|>,$|>$" )
                            );
                    
                    BOOST_FOREACH( string str, result )
                    {
                        cout<< str << endl;
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
