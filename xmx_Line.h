#ifndef XMX_LINE_H
#define XMX_LINE_H
#include <GL/glu.h>

class xmx_Line
{
public:
    xmx_Line( int, int, int, int );
    xmx_Line( float, float, float, float );
    ~xmx_Line();
    
    void print();
    void draw();
    void rotate( float );
    virtual GLfloat getMaxX();
    virtual GLfloat getMinX();
    virtual GLfloat getMaxY();
    virtual GLfloat getMinY();

    GLfloat x1;
    GLfloat y1;
    GLfloat x2;
    GLfloat y2;
};

#endif /* XMX_LINE_H */
