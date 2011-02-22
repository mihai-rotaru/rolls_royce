#ifndef UTILS_H
#define UTILS_H

#include <list>
#include <string>
using namespace std;

#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string/regex.hpp>

namespace xmx {
class Shape;
class BezierCurve;

struct Color
{ 
    GLfloat R,G,B;
    Color( GLfloat r, GLfloat g, GLfloat b ) : R(r), G(g), B(b) {}
    Color() : R( 0.5f ), G( 0.5f ), B( 0.5f ) {}
};

struct basicAnimation
{
    int frame_to_render;
    int total_frames;
    bool active;
    bool repeat;
};


//void loadPovFile( string filename, Shape& shape );
void printText( int nX, int nY, string text );
void printBigText( int nX, int nY, string text );
void printFeedbackBuffer( GLfloat* buff, GLint size );
template < typename T > T getFromRegex( string , const boost::regex& );
void morphBeziers( BezierCurve& , BezierCurve&, basicAnimation& );
GLfloat dist( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );

} // namespace xmx
#endif /* UTILS_H */
