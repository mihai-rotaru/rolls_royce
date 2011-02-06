#include <iostream>
#include <memory>
using namespace std;

#include "BezierPath.h"
#include <GL/glu.h>

#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>

namespace xmx {

void xmx::BezierPath::addCurve( 
        GLfloat e1_x, GLfloat e1_y,
        GLfloat c1_x, GLfloat c1_y,
        GLfloat c2_x, GLfloat c2_y,
        GLfloat e2_x, GLfloat e2_y )
{
    boost::shared_ptr<BezierCurve> mptr
        ( new BezierCurve( e1_x, e1_y, c1_x, c1_y, c2_x, c2_y, e2_x, e2_y ));

    curves.push_front( mptr );
}


void xmx::BezierPath::draw()
{
    BOOST_FOREACH( boost::shared_ptr<BezierCurve> curve_ptr, curves )
    {
        curve_ptr->draw();
    }
}


void xmx::BezierPath::print()
{
    cout<<"BezierPath @ "<<this<<endl;
    BOOST_FOREACH( boost::shared_ptr<BezierCurve> curve_ptr, curves )
    {
        curve_ptr->draw();
    }
}


void xmx::BezierPath::resizeTo( int newWidth )
{
    return /* something */;
}

xmx::BezierPath::BezierPath()
{
}

xmx::BezierPath::~BezierPath()
{
}

} // namespace xmx
