#ifndef XMX_LINE_H
#define XMX_LINE_H
#include <GL/glu.h>

namespace xmx {

class Line
{
public:
    Line( int, int, int, int );
    Line( float, float, float, float );
    ~Line();
    
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

} // namespace xmx
#endif /* XMX_LINE_H */
