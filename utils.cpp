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
typedef boost::shared_ptr<Shape> sptrShape;

namespace xmx {

struct Color;

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

template int getFromRegex< int >( string haystack, const boost::regex& myRegex );
template string getFromRegex< string >( string haystack, const boost::regex& myRegex );

void printText( int nX, int nY, string text )
{
    int lines;
    const char *pszText = text.c_str();
    const char *p;

    glColor3ub( 60, 60, 60 );
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
        cout<< endl << "checking if token at index " << size-count << " in " << buffer << endl;
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
            cout << "UNIDENTIFIED TOKEN" << endl;
            cout << "count = "<< count << ", index: " << size-count;
            cout << "value = " << buffer[ size - count ] << endl;
        }
    }

}
} // namespace xmx
