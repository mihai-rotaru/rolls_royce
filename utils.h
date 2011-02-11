#ifndef UTILS_H
#define UTILS_H

#include <list>
#include <string>
using namespace std;

#include <boost/shared_ptr.hpp>

#include "Shape.h"
using namespace xmx;

void loadPovFile( string filename, list< boost::shared_ptr< Shape > > shape_list );

#endif /* UTILS_H */
