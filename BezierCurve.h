#ifndef XMX_BEZIER_H
#define XMX_BEZIER_H
#include <GL/glu.h>
#include "globals.h"
#include "Point.h"
#include "Primitive.h"

#include <string>

namespace xmx {

class BezierCurve : virtual Primitive
{
    public:
        // constructor parameters:
        // e1,e2: end points
        // c1,c2: control points
        BezierCurve( Point e1, Point c1, Point c2, Point e2 );
        BezierCurve( GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat );
        ~BezierCurve();

        // methods for Primitive
        void print();
        void print( std::string str );
        void draw();
        void rotate( GLfloat );
        void rotate( GLfloat, GLfloat, GLfloat );
        void move( GLfloat, GLfloat );
        void scale( GLfloat, GLfloat );
        
        void drawBoundingBox();

        GLfloat getMaxX();
        GLfloat getMinX();
        GLfloat getMaxY();
        GLfloat getMinY();
        GLfloat distanceTo( GLfloat, GLfloat );

        // array holding the vertices
        GLfloat points[4][3];

        // sugar
        const static int END_PT_1 = 0;
        const static int CTRL_PT_1 = 1;
        const static int CTRL_PT_2 = 2;
        const static int END_PT_2 = 3;
};

} // namespace xmx
#endif /* XMX_BEZIER_H */
