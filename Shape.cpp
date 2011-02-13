#include <iostream>
#include <fstream>
#include <typeinfo>
using namespace std;

#include <GL/glu.h>

#include <boost/shared_ptr.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/foreach.hpp>

#include "globals.h"
#include "Point.h"
#include "Primitive.h"
#include "Shape.h"
#include "BezierCurve.h"
#include "Line.h"

namespace xmx {

int Shape::count = 0;

Shape::Shape()
{
    if( DEBUG_CTOR_DTOR ) cout<<"ctor for Shape @ " << this << endl;
    isBezier = false;
    Shape::count++;
}

Shape::~Shape()
{
    if( DEBUG_CTOR_DTOR ) cout<<"dtor for Shape @ " << this << " ( " << name <<" )" << endl;
    Shape::count--;
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

void Shape::addBezierCurve(
        Point& P0, Point& P1, Point& P2, Point& P3
        )
{
    boost::shared_ptr<BezierCurve> mptr
        ( new BezierCurve( P0, P1, P2, P3 ));

    primitives.push_front( mptr );
}

void Shape::addBezierCurve( BezierCurve* ptr_bc )
{
    boost::shared_ptr< BezierCurve > mptr( ptr_bc );
    primitives.push_front( mptr );
}

void Shape::addBezierCurve( boost::shared_ptr< BezierCurve > sptr )
{
    primitives.push_front( sptr );
}

void Shape::addLine( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 )
{
    boost::shared_ptr< Line > mptr
        ( new Line( x1, y1, x2, y2 ));

    primitives.push_front( mptr );
    isBezier = false;
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
    if ( DEBUG_TRANSF_MOVE ) cout << "moving Shape @ " << this << endl;
    BOOST_FOREACH( boost::shared_ptr< Primitive > pPrimitive, primitives )
    {
        pPrimitive->move( x_dir, y_dir );
    }
}

void Shape::draw()
{
    if( isBezier )
    {
        GLint total_lines = 0;

        // how many lines in this shape ? ( the sum of lines for all BC's contained )
        if( DEBUG_SHAPE_BEZIER_DRAW ) cout<<"counting lines composing Shape @ " << this << " ( " << name << " )" << endl; 
        BOOST_FOREACH( boost::shared_ptr< Primitive > pPrimitive, primitives )
        {
            BezierCurve* bc = dynamic_cast< BezierCurve* > ( pPrimitive.get() );
            if( bc != NULL )
            {
                if( DEBUG_SHAPE_BEZIER_DRAW ) cout<<"counting lines composing BezierCurve @ " << bc << " ( " << bc->name << " ) ... "; 
                GLint bc_lines = bc-> getMaxX() - bc -> getMinX();
                if ( bc_lines == 0 ) bc->isLine();
                total_lines += bc_lines;
                if( DEBUG_SHAPE_BEZIER_DRAW ) cout<< bc_lines << " ( new total: " << total_lines << " )" << endl;
            }
        }

        // gather the lines which form all Bezier curves in this Shape
        BOOST_FOREACH( boost::shared_ptr< Primitive > pPrimitive, primitives )
        {
            BezierCurve* bc = dynamic_cast< BezierCurve* > ( pPrimitive.get() );
            if( bc != NULL )
            {
                GLint num_lines = bc-> getMaxX() - bc -> getMinX();
                bc->draw();

            }
        }
    }

    else // contains other types of primitives ( not only beziers )
    {
        BOOST_FOREACH( boost::shared_ptr< Primitive > pPrimitive, primitives )
        {
            pPrimitive->draw();
        }
    }
}

GLfloat Shape::distanceTo( GLfloat x, GLfloat y )
{
    return -1001;
}

} // namespace xmx
