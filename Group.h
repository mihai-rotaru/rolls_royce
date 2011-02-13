#ifndef GROUP_H
#define GROUP_H

#include <string>
#include <GL/glu.h>
#include <boost/shared_ptr.hpp>
#include "Shape.h"

namespace xmx {

class Group
{
public:
    std::string name;
    list< boost::shared_ptr<Shape    > > shapes;
    int getNumberOfShapes()      { return shapes.size();     };

    void addShape( Shape& shape );
    void loadFromPovFile( std::string );
    void draw();
};

} // namespace xmx
#endif /* GROUP_H */
