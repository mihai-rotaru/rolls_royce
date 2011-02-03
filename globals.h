#ifndef GLOBALS_H
#define GLOBALS_H

extern int window_width;
extern int window_height;

extern bool XMX_SHOW_BEZIER_POINTS;

struct xPoint
{
    GLfloat x,y,z;
    xPoint( GLfloat _x, GLfloat _y, GLfloat _z ): x(_x), y(_y), z(_z)
    {}
};

typedef xPoint GL_3DPointf;

#endif /* GLOBALS_H */
