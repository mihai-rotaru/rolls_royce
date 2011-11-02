#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
using namespace std;

#include <GL/glut.h>
#include <GL/glu.h>

#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

#include "globals.h"
#include "utils.h"
#include "Point.h"
#include "Line.h"
#include "BezierCurve.h"
#include "Shape.h"
#include "Group.h"
#include "Layer.h"
using namespace xmx;

Line my_line( 100, 150, 200, 300 );
typedef boost::shared_ptr<Shape> sptrShape;

string build_info;
string strFPS;

// bezier stuff
Point ae1( -50, 95  );
Point ac1( 232, -50 );
Point ac2( 737, 151 );
Point ae2( 908, 15 );
Point e1( 710,223 );
Point c1( 7, 190 );
Point c2( 835,190 );
Point e2( 145,225 );
BezierCurve bez_path ( e1, c1, c2, e2 );
BezierCurve bez_path2( ae1, ac1, ac2, ae2 );
Line line( e1, e2 );

// the count of frames rendered so far
GLint frame;

//
Group world;

// some colors
Color text_col(0,0,0);
Color logo_color( 1,1,1 );

void myDisplayFunc( void )
{
    glClear( GL_COLOR_BUFFER_BIT );
    glRenderMode( GL_RENDER );

    // set line colour red( r=1, g=0,b=0 ).
    glColor3f( dcol.R, dcol.G, dcol.B );

    frame++;
    if( SHOW_FRAME_NUMBER ) cout<< endl << "FRAME: "<< frame << endl;

    // bez_path.draw();
//    bez_path2.draw();
    world.draw();

    // version and build info, FPS
    glColor3ub( 60, 60, 60 );
    printText( 10, glutGet( GLUT_WINDOW_HEIGHT ) - 18, VERSION );
    printText( 10, glutGet( GLUT_WINDOW_HEIGHT ) - 32, build_info );
    if( SHOW_FPS ) printText( glutGet( GLUT_WINDOW_WIDTH ) - 50, glutGet( GLUT_WINDOW_HEIGHT ) - 18, strFPS ); 

    glutSwapBuffers();
    
    if( frame > 40000 ) frame = 0;
}

void init( void )
{
    // select clearing color
    glClearColor( dcol_Clear.R, dcol_Clear.G, dcol_Clear.B, 0.0 );

    // initialize viewing values
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, GLUT_WINDOW_HEIGHT, 0, GLUT_WINDOW_WIDTH );

    // read version number and build info
    ifstream ver_file( "VERSION", ifstream::in );
    getline( ver_file, VERSION );
    VERSION = "World Statistics, " + VERSION;
    getline( ver_file, BUILD_ID );
    getline( ver_file, BUILD_TIME );
    build_info = "Build info: " + BUILD_ID + " @ " + BUILD_TIME;

    // display FPS
    stringstream ss;
    ss << FPS;
    strFPS = "FPS: " + ss.str();

    // load map of the world
    world.name = "Map of the world";
    world.loadFromPovFile( "vector/blank-world-robinson.pov" );
    world.setColor( 0.5, 0.5, 0.5 );
}

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch( key )
    {
        case 'j':
            world.move (  0, -5 );
            break;
        case 'k':
            world.move (  0,  5 );
            break;
        case 'l':
            world.move (  5,  0 );
            break;
        case 'h':
            world.move ( -5,  0 );
            break;
        case 27:
            exit(0);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void myReshape( int nWidht, int nHeight )
{
    glViewport( 0, 0, (GLsizei)nWidht, (GLsizei)nHeight );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, nWidht, 0, nHeight);
    glutPostRedisplay();
}

void Timer( int value )
{
    if( value ) glutPostRedisplay();
    glutTimerFunc( 1000/FPS, Timer, value );
}


void visibility(int state)
{
    switch (state)
    {
        case GLUT_VISIBLE:
            Timer(1);
            break;
        case GLUT_NOT_VISIBLE:
            Timer(0);
            break;
        default:
            break;
    }
}

int main( int argc, char** argv )
{
    // gl stuff
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE );

    // window stuff
    window_width = 840;
    window_height = 480;
    glutInitWindowSize( window_width, window_height );
    glutInitWindowPosition( 100, 100 );
    glutCreateWindow( "World Statistics" );
    init();

    // callbacks
    glutDisplayFunc( myDisplayFunc );
    glutKeyboardFunc( myKeyboardFunc );
    glutReshapeFunc( myReshape );
    glutVisibilityFunc( visibility );

    glutMainLoop();
    return 0;   // ANSI C requires main to return int.
}
