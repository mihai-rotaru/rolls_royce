#include <GL/glu.h>
#include <boost/shared_ptr.hpp>

#include "Primitive.h"
#include "Shape.h"
#include "BezierCurve.h"

void xmx::Shape::addBezierCurve(
        GLfloat e1_x, GLfloat e1_y,
        GLfloat c1_x, GLfloat c1_y,
        GLfloat c2_x, GLfloat c2_y,
        GLfloat e2_x, GLfloat e2_y )
{
    boost::shared_ptr<BezierCurve> mptr
        ( new BezierCurve( e1_x, e1_y, c1_x, c1_y, c2_x, c2_y, e2_x, e2_y ));

    primitives.push_front( mptr );
}


void xmx::Shape::addLine( GLfloat, GLfloat, GLfloat, GLfloat )
{
    return /* something */;
}

xmx::Shape::Shape()
{
}

xmx::Shape::~Shape()
{
}
