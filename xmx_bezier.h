#ifndef XMX_BEZIER_H
#define XMX_BEZIER_H
#include <GL/glu.h>
#include "globals.h"

class xmx_BezierPath
{
    public:
        // constructor parameters:
        // e1,e2: end points
        // c1,c2: control points
        xmx_BezierPath( GL_3DPointf& e1, GL_3DPointf& c1, GL_3DPointf& c2, GL_3DPointf& e2 );
        ~xmx_BezierPath();

        // methods
        void print();
        void draw();
        void rotate( float );

        // array holding the vertices
        GLfloat points[4][3];

        // sugar
        const static int END_PT_1 = 0;
        const static int CTRL_PT_1 = 1;
        const static int CTRL_PT_2 = 2;
        const static int END_PT_2 = 3;
};

#endif /* XMX_BEZIER_H */
