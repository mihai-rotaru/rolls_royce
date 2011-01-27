#include <GL/glu.h>
#include "xmx_Line.h"

xmx_Line::xmx_Line( float _x1, float _y1, float _x2, float _y2 )
{
    x1 = ( GLfloat ) _x1;
    y1 = ( GLfloat ) _y1;
    x2 = ( GLfloat ) _x2;
    y2 = ( GLfloat ) _y2;

}

xmx_Line::xmx_Line( int _x1, int _y1, int _x2, int _y2 )
{
    x1 = ( GLfloat ) _x1;
    y1 = ( GLfloat ) _y1;
    x2 = ( GLfloat ) _x2;
    y2 = ( GLfloat ) _y2;
}

xmx_Line::~xmx_Line()
{
}

void xmx_Line::draw()
{
    glBegin( GL_LINES );
        glVertex2f( x1, y1 );
        glVertex2f( x2, y2 );
    glEnd();
}

void xmx_Line::print()
{
}

