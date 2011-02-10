#ifndef XMX_LINE_H
#define XMX_LINE_H

#include <string>
#include <GL/glu.h>
#include "Primitive.h"

namespace xmx {

class Line : virtual public Primitive
{
public:
    Line( int, int, int, int );
    Line( GLfloat, GLfloat, GLfloat, GLfloat );
    ~Line();
    
    std::string name;

    void move( GLfloat, GLfloat );
    void scale( GLfloat, GLfloat );
    void rotate( GLfloat );
    void rotate( GLfloat, GLfloat, GLfloat );
    void draw();
    void print();
    void print( std::string );
    GLfloat distanceTo( GLfloat, GLfloat );

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
