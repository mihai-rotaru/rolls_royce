#include <GL/glu.h>
#include "globals.h"
//#include "Point.h"
//using namespace xmx;
#include <string>

// these are set in the main function, when the window is created
int window_width =  0;
int window_height = 0;

std::string VERSION    = "?";
std::string BUILD_ID   = "?";
std::string BUILD_TIME = "?";

// show points for bezier curves ?
bool SHOW_BEZIER_POINTS       = false;
bool SHOW_BEZIER_BOUNDING_BOX = false;

// debug flags
bool DEBUG_CTOR_DTOR                = true;
bool DEBUG_BEZIER_BOUNDING_BOX      = false;
bool DEBUG_TRANSF_ROTATE            = false;
bool DEBUG_TRANSF_MOVE              = false;
bool DEBUG_TRANSF_SCALE             = false;
bool DEBUG_INIT_BEZIER_BOUNDING_BOX = false;
bool DEBUG_POV_LOADING              = true;

bool STRICT_POV                     = true;
