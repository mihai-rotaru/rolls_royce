#ifndef GLOBALS_H
#define GLOBALS_H

extern int window_width;
extern int window_height;

extern bool XMX_SHOW_BEZIER_POINTS;

struct xPoint
{
    GLfloat x,y,z;
    xPoint(): x( 0.0f ), y( 0.0f ), z( 0.0f ) {}
    xPoint( GLfloat _x, GLfloat _y, GLfloat _z ): x(_x), y(_y), z(_z) {}
    xPoint( GLfloat _x, GLfloat _y ): x(_x), y(_y), z( 0 ) {}
};

typedef xPoint Point;

#endif /* GLOBALS_H */
