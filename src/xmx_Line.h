#ifndef XMX_LINE_H
#define XMX_LINE_H
#include <GL/glu.h>

class xmx_Line
{
public:
    xmx_Line();
    xmx_Line( int, int, int, int );
    xmx_Line( float, float, float, float );
    xmx_Line( GLfloat, GLfloat, GLfloat, GLfloat );
    ~xmx_Line();
    
    void print();

    GLfloat x1;
    GLfloat y1;
    GLfloat x2;
    GLfloat y2;
};

#endif /* XMX_LINE_H */
