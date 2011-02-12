#include <GL/glu.h>
#include "globals.h"
#include "utils.h"
//#include "Point.h"
//using namespace xmx;
#include <string>

// these are set in the main function, when the window is created
int window_width =  0;
int window_height = 0;

// these are read from the VERSION file at compile time
std::string VERSION    = "?";
std::string BUILD_ID   = "?";
std::string BUILD_TIME = "?";

// show points for bezier curves ?
bool SHOW_BEZIER_POINTS             = true;
bool SHOW_BEZIER_BOUNDING_BOX       = true;
bool SHOW_BEZIER_CONTROL_POINT_LINES= true;

// debug flags
bool DEBUG_CTOR_DTOR                = false;
bool DEBUG_BEZIER_BOUNDING_BOX      = false;
bool DEBUG_TRANSF_ROTATE            = false;
bool DEBUG_TRANSF_MOVE              = false;
bool DEBUG_TRANSF_SCALE             = false;
bool DEBUG_INIT_BEZIER_BOUNDING_BOX = false;
bool DEBUG_POV_LOADING              = false;
bool DEBUG_FEEDBACK_TOKENS          = false;

bool STRICT_POV                     = true;

// default colors
Color dcol_CPoints     ( 0.0f, 0.5f, 0.0f );
Color dcol_EPoints     ( 0.0f, 0.0f, 0.5f );
Color dcol_ECLines     ( 0.5f, 0.5f, 0.5f );
Color dcol_BezierBB    ( 0.5f, 0.1f, 0.7f );
Color dcol             ( 1.0f, 0.0f, 0.0f );
Color dcol_Clear       ( 0.0f, 0.0f, 0.0f );
