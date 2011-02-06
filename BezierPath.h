#ifndef BEZIERPATH_H
#define BEZIERPATH_H

#include <iostream>
#include <list>
using namespace std;

#include <GL/glu.h>
#include "BezierCurve.h"
using namespace xmx;

#include <boost/shared_ptr.hpp>
//typedef boost::shared_ptr<BezierCurve> BezierCurvePtr;

namespace xmx {

class BezierPath
{
public:
    BezierPath();
    ~BezierPath();
    
    list< boost::shared_ptr<BezierCurve> > curves;

    void addCurve( GLfloat e1_x, GLfloat e1_y,
                   GLfloat c1_x, GLfloat c1_y,
                   GLfloat c2_x, GLfloat c2_y,
                   GLfloat e2_x, GLfloat e2_y );
    void draw();
    void print();
    void resizeTo( int newWidth );
};

} // namespace xmx
#endif /* BEZIERPATH_H */
