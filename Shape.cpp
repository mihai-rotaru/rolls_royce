#include <iostream>
using namespace std;

#include <GL/glu.h>

#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

#include "globals.h"
#include "Primitive.h"
#include "Shape.h"
#include "BezierCurve.h"
#include "Line.h"

namespace xmx {

Shape::Shape()
{
    if( DEBUG_CTOR_DTOR ) cout<<"ctor for Shape @ " << this << endl;
}

Shape::~Shape()
{
    if( DEBUG_CTOR_DTOR ) cout<<"dtor for Shape @ " << this << endl;
}

void Shape::addBezierCurve(
        GLfloat e1_x, GLfloat e1_y,
        GLfloat c1_x, GLfloat c1_y,
        GLfloat c2_x, GLfloat c2_y,
        GLfloat e2_x, GLfloat e2_y )
{
    boost::shared_ptr<BezierCurve> mptr
        ( new BezierCurve( e1_x, e1_y, c1_x, c1_y, c2_x, c2_y, e2_x, e2_y ));

    primitives.push_front( mptr );
}


void Shape::addLine( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 )
{
    boost::shared_ptr< Line > mptr
        ( new Line( x1, y1, x2, y2 ));

    primitives.push_front( mptr );
}


void Shape::scale( GLfloat x_dir, GLfloat y_dir )
{
    return;
}

void Shape::rotate( GLfloat )
{
    return /* something */;
}

void Shape::rotate( GLfloat, GLfloat, GLfloat )
{
    return /* something */;
}

void Shape::move( GLfloat x_dir, GLfloat y_dir )
{
    cout << "moving Shape @ " << this << endl;
    BOOST_FOREACH( boost::shared_ptr< Primitive > pPrimitive, primitives )
    {
        pPrimitive->move( x_dir, y_dir );
    }
}

void Shape::draw()
{
    BOOST_FOREACH( boost::shared_ptr< Primitive > pPrimitive, primitives )
    {
        pPrimitive->draw();
    }
}

GLfloat Shape::distanceTo( GLfloat x, GLfloat y )
{
    return -1001;
}

} // namespace xmx
