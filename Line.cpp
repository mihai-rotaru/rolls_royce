#include <GL/glu.h>

#include <iostream>
#include <iomanip>
using namespace std;

#include "globals.h"
#include "Line.h"

namespace xmx {

Line::Line( float _x1, float _y1, float _x2, float _y2 )
{
    x1 = ( GLfloat ) _x1;
    y1 = ( GLfloat ) _y1;
    x2 = ( GLfloat ) _x2;
    y2 = ( GLfloat ) _y2;

}

Line::Line( int _x1, int _y1, int _x2, int _y2 )
{
    x1 = ( GLfloat ) _x1;
    y1 = ( GLfloat ) _y1;
    x2 = ( GLfloat ) _x2;
    y2 = ( GLfloat ) _y2;
}

Line::~Line()
{
}

void Line::rotate( float theta )
{
    cout<<"rotating..."<<endl;
    GLfloat* buff = new GLfloat[10];
    glFeedbackBuffer( 10, GL_2D, buff );
    GLfloat x_rel = ( (getMaxX() - getMinX())/2 + getMinX() );
    GLfloat y_rel = ( (getMaxY() - getMinY())/2 + getMinY() );

    // save coordinates before rotation
    GLfloat before_x1 = x1;
    GLfloat before_y1 = y1;
    GLfloat before_x2 = x2;
    GLfloat before_y2 = y2;

    cout<<"before rotation:"<<endl;
    print();

    glRenderMode( GL_FEEDBACK );

    glPushMatrix();
        glTranslatef( x_rel, y_rel, 0 );
        glRotatef( theta, 0, 0, 1 );
        glTranslatef( -x_rel, -y_rel, 0 );
        draw();
    glPopMatrix();

    // TODO: check if new coordinates are out of bounds

    x1 = buff[1];
    y1 = buff[2];
    x2 = buff[3];
    y2 = buff[4];

    cout<<"after rotation:"<<endl;
    print();

    //cleanup
    delete[] buff;

    cout<<"rotated."<<endl;
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
    cout<<" Line, address: "<<this<<endl;
    cout<< " x1: "<< setfill(' ') << setw( 10 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) <<x1;
    cout<< " y1: "<< setfill(' ') << setw( 10 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) <<y1;
    cout<< " x2: "<< setfill(' ') << setw( 10 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) <<x2;
    cout<< " y2: "<< setfill(' ') << setw( 10 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) <<y2;
    cout<<endl;
}

GLfloat Line::getMaxX() { return ( x1 > x2 ? x1 : x2 ); }
GLfloat Line::getMinX() { return ( x1 < x2 ? x1 : x2 ); }
GLfloat Line::getMaxY() { return ( y1 > y2 ? y1 : y2 ); }
GLfloat Line::getMinY() { return ( y1 < y2 ? y1 : y2 ); }

} // namespace xmx
