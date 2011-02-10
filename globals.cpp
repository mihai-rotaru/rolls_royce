#include <GL/glu.h>
#include "globals.h"
//#include "Point.h"
//using namespace xmx;

// these are set in the main function, when the window is created
int window_width =  0;
int window_height = 0;

// show points for bezier curves ?
bool SHOW_BEZIER_POINTS       = false;
bool SHOW_BEZIER_BOUNDING_BOX = false;

// debug flags
bool DEBUG_CTOR_DTOR                = true;
bool DEBUG_BEZIER_BOUNDING_BOX      = false;
bool DEBUG_INIT_BEZIER_BOUNDING_BOX = true;
