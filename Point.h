#ifndef POINT_H
#define POINT_H

#include <GL/glu.h>

namespace xmx {

class Point
{
public:
    GLfloat x,y,z;
    Point(): x( 0.0f ), y( 0.0f ), z( 0.0f ) {}
    Point( GLfloat _x, GLfloat _y, GLfloat _z ): x(_x), y(_y), z(_z) {}
    Point( GLfloat _x, GLfloat _y ): x(_x), y(_y), z( 0 ) {}
    const Point operator+( const Point& rhs )   const { return Point( x + rhs.x, y + rhs.y, z + rhs.z ); }
    const Point operator-( const Point& rhs )   const { return Point( x - rhs.x, y - rhs.y, z - rhs.z ); }
    const Point operator*( const Point& rhs )   const { return Point( x * rhs.x, y * rhs.y, z * rhs.z ); }
    const Point operator/( const Point& rhs )   const { return Point( x / rhs.x, y / rhs.y, z / rhs.z ); }
    const Point operator+( const GLfloat& rhs )  const { return Point( x + rhs, y + rhs, z + rhs ); }
    const Point operator-( const GLfloat& rhs )  const { return Point( x - rhs, y - rhs, z - rhs ); }
    const Point operator*( const GLfloat& rhs )  const { return Point( x * rhs, y * rhs, z * rhs ); }
    const Point operator/( const GLfloat& rhs )  const { return Point( x / rhs, y / rhs, z / rhs ); }
    void print();
};

} // namespace xmx
#endif /* POINT_H */
