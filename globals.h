#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>

extern int window_width;
extern int window_height;

extern std::string VERSION;
extern std::string BUILD_ID;
extern std::string BUILD_TIME;

extern bool SHOW_BEZIER_POINTS;
extern bool SHOW_BEZIER_BOUNDING_BOX;

extern bool DEBUG_CTOR_DTOR;
extern bool DEBUG_TRANSF_ROTATE;
extern bool DEBUG_TRANSF_MOVE;
extern bool DEBUG_TRANSF_SCALE;
extern bool DEBUG_BEZIER_BOUNDING_BOX;
extern bool DEBUG_INIT_BEZIER_BOUNDING_BOX;
extern bool DEBUG_POV_LOADING;
extern bool DEBUG_FEEDBACK_TOKENS;

extern bool STRICT_POV;

#endif /* GLOBALS_H */
