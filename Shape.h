#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include <list>
using namespace std;

#include <GL/glu.h>
#include <boost/shared_ptr.hpp>
#include "Primitive.h"

namespace xmx {

class Shape
{
public:
    Shape();
    ~Shape();

    list< boost::shared_ptr<Primitive> > primitives;

    bool isSelected;
    string name;

    void addLine( GLfloat, GLfloat, GLfloat, GLfloat );
    void addBezierCurve( GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);

};

} // namespace xmx
#endif /* SHAPE_H */
