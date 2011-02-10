#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include <list>
using namespace std;

#include <GL/glu.h>
#include <boost/shared_ptr.hpp>

#include "Point.h"
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
    void addBezierCurve( Point&, Point&, Point&, Point& );

    void move( GLfloat, GLfloat );
    void scale( GLfloat, GLfloat );
    void rotate( GLfloat );
    void rotate( GLfloat, GLfloat, GLfloat );
    void print();
    void print( std::string );
    void draw();
    GLfloat distanceTo( GLfloat, GLfloat );

};

Shape loadFromPovFile( string filename );

} // namespace xmx
#endif /* SHAPE_H */
