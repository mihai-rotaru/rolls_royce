#include <iostream>
#include "Point.h"

namespace xmx {

void Point::print()
{
    std::cout<<"Point @ "<< this <<"x: "<< x <<"y: "<< y <<std::endl;
}

} // namespace xmx
