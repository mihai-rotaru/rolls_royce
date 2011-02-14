#include <iostream>
#include <iomanip>
using namespace std;

#include <GL/glu.h>

#include "globals.h"
#include "Line.h"

namespace xmx {

Line::Line( float _x1, float _y1, float _x2, float _y2 )
{
    if( DEBUG_CTOR_DTOR ) cout<<"ctor for Line @ " << this << endl;
    x1 = ( GLfloat ) _x1;
    y1 = ( GLfloat ) _y1;
    x2 = ( GLfloat ) _x2;
    y2 = ( GLfloat ) _y2;

}

Line::Line( int _x1, int _y1, int _x2, int _y2 )
{
    if( DEBUG_CTOR_DTOR ) cout<<"ctor for Line @ " << this << endl;
    x1 = ( GLfloat ) _x1;
    y1 = ( GLfloat ) _y1;
    x2 = ( GLfloat ) _x2;
    y2 = ( GLfloat ) _y2;
}

Line::~Line()
{
    if( DEBUG_CTOR_DTOR ) cout<<"dtor for Line @ " << this << endl;
}

void Line::scale( GLfloat x_dir, GLfloat y_dir )
{

}

void print_buffer( GLfloat* buffer, int size )
{
    for( int i = 0; i < size; i ++ )
    {
        cout<<"at position "<<i<<": "<< buffer[i] <<endl;
    }
}

void Line::move( GLfloat x_dir, GLfloat y_dir )
{
    x1 += x_dir;
    y1 += y_dir;
    x2 += x_dir;
    y2 += y_dir;
}


void Line::rotate( float theta )
{
    cout<<endl<<"rotating Line @ "<< this << endl;

    GLfloat* buff = new GLfloat[10];
    glFeedbackBuffer( 10, GL_2D, buff );

    GLfloat x_rel = ( (getMaxX() - getMinX())/2 + getMinX() );
    GLfloat y_rel = ( (getMaxY() - getMinY())/2 + getMinY() );

    // save coordinates before rotation
    GLfloat before_x1 = x1;
    GLfloat before_y1 = y1;
    GLfloat before_x2 = x2;
    GLfloat before_y2 = y2;

    cout<<"before rotation: ";
    print("");

    glRenderMode( GL_FEEDBACK );

    glPushMatrix();
        glTranslatef( x_rel, y_rel, 0 );
        glRotatef( theta, 0, 0, 1 );
        glTranslatef( -x_rel, -y_rel, 0 );
        draw();
    glPopMatrix();

    glFlush();
    // TODO: check if new coordinates are out of bounds

    x1 = buff[1];
    y1 = buff[2];
    x2 = buff[3];
    y2 = buff[4];

    cout<<"after rotation:  ";
    print("");

    //cleanup
    delete[] buff;

    glRenderMode( GL_RENDER );

}

void Line::rotate( GLfloat theta, GLfloat x, GLfloat y)
{

}

void Line::draw()
{
    glBegin( GL_LINES );
        glVertex2f( x1, y1 );
        glVertex2f( x2, y2 );
    glEnd();
}

void Line::print()
{
    cout<<" Line @ "<<this<<": ";
    cout<< " x1: "<< setfill(' ') << setw( 10 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) <<x1;
    cout<< " y1: "<< setfill(' ') << setw( 10 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) <<y1;
    cout<< " x2: "<< setfill(' ') << setw( 10 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) <<x2;
    cout<< " y2: "<< setfill(' ') << setw( 10 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) <<y2;
    cout<<endl;
}

void Line::print( std::string str )
{
    cout<< str ;
    cout<< " x1: "<< setfill(' ') << setw( 10 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) <<x1;
    cout<< " y1: "<< setfill(' ') << setw( 10 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) <<y1;
    cout<< " x2: "<< setfill(' ') << setw( 10 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) <<x2;
    cout<< " y2: "<< setfill(' ') << setw( 10 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) <<y2;
    cout<<endl;
}

GLfloat Line::distanceTo( GLfloat x, GLfloat y )
{
    return -1001;
}

GLfloat Line::getMaxX() { return ( x1 > x2 ? x1 : x2 ); }
GLfloat Line::getMinX() { return ( x1 < x2 ? x1 : x2 ); }
GLfloat Line::getMaxY() { return ( y1 > y2 ? y1 : y2 ); }
GLfloat Line::getMinY() { return ( y1 < y2 ? y1 : y2 ); }

} // namespace xmx
