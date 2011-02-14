#include <iostream>
#include <fstream>
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
struct Animation;

string build_info;

// bezier stuff
Point e1(  30, 70  );
Point c1(   0, 270 );
Point c2( 290, 110 );
Point e2( 200, 100 );
BezierCurve bez_path( e1, c1, c2, e2 );

// groups - loaded from .pov files
Group rolls;
Group since1904;

// the count of frames rendered so far
GLint frame;

struct Animation
{
    GLint type;            // movement, rotation, scaling, color
    GLint duration;        // duration - in frames
    GLint currentFrame;
    bool repeat;         // repeat ad infinitum ?
    GLfloat q1;
    GLfloat q2;
    GLfloat q3;
    void print();
};

// animations
Animation a1,a2;

void Animation::print()
{
    cout << "Animation struct @ " << this << endl;
    cout <<"    type: "           << type << endl;
    cout <<"    duration: "           << duration << endl;
    cout <<"    currentFrame: "           << currentFrame << endl;
    cout <<"    repeat: "           << repeat << endl;
    cout <<"    q1: "           << q1 << endl;
    cout <<"    q2: "           << q2 << endl;
    cout <<"    q3: "           << q3 << endl;
}

void performAnimation( Group* g, Animation& a )
{
    GLint type         = a.type;
    GLint currentFrame = a.currentFrame;
    GLint duration     = a.duration;
    GLint repeat       = a.repeat;
    GLfloat q1         = a.q1;
    GLfloat q2         = a.q2;
    GLfloat q3         = a.q3;

    if( !repeat && currentFrame == duration ) return;

    if( DEBUG_ANIMATIONS )
    {
        cout <<"animation for group " << g << " ( " << g -> name << " )" << endl;
        a.print();
    }

    a.currentFrame++;

    if( type == 1 ) // movement ( translation )
    {
        // q1 - x movement
        // q2 - y movement
        // q3 - not used

        GLfloat mx_dir = q1/duration;
        GLfloat my_dir = q2/duration;

        if( DEBUG_ANIMATIONS )
        {
            cout << "   Movement animation in animation struct @ " << &a << endl;
            cout << "   moving by: x_dir:  " << mx_dir << ",  my_dir:  " << my_dir << endl;

        }
        g -> move( mx_dir, my_dir );
    }

    else if( type == 2 ) // rotation
    {
        // q1 - how many degrees to rotate
        // q2 - not used
        // q3 - not used
        g -> rotate( q1/duration * currentFrame );
    }

    else if( type == 3 ) // scaling
    {
        // q1 - x scaling
        // q2 - y scaling
        // q3 - not used
        g -> scale( q1/duration * currentFrame, q2/duration * currentFrame );
    }
    
    else if( type == 4 ) // color
    {
        if( DEBUG_ANIMATIONS )
            cout << "old color: R = " << g -> color.R <<" ; G = " << g -> color.G <<" ; B = " << g -> color.B << endl; 
        // q1 - target R
        // q2 - target G
        // q3 - target B

        GLfloat rdiff = q1 - g -> color.R;
        GLfloat gdiff = q1 - g -> color.R;
        GLfloat bdiff = q1 - g -> color.R;

        if( DEBUG_ANIMATIONS )
        {
            cout <<" rdiff = " << rdiff << endl;
            cout <<" gdiff = " << gdiff << endl;
            cout <<" bdiff = " << bdiff << endl;
        }
        g -> setColor( 
                g -> color.R + (GLfloat)( q1 - g -> color.R )/( duration - currentFrame ), 
                g -> color.G + (GLfloat)( q2 - g -> color.G )/( duration - currentFrame ),
                g -> color.B + (GLfloat)( q3 - g -> color.B )/( duration - currentFrame )
                );

        if( DEBUG_ANIMATIONS )
            cout << "new color: R = " << g -> color.R <<" ; G = " << g -> color.G <<" ; B = " << g -> color.B << endl; 
    }

    if( currentFrame == duration )
        currentFrame = 0;
}


void myDisplayFunc( void )
{
    glClear( GL_COLOR_BUFFER_BIT );
    glRenderMode( GL_RENDER );

    // set line colour red( r=1, g=0,b=0 ).
    glColor3f( dcol.R, dcol.G, dcol.B );

    frame++;

//    my_line.draw();
//    my_line.rotate(1);
    
    performAnimation( &rolls, a1 );
    performAnimation( &rolls, a2 );
    rolls.draw();
    since1904.draw();

    printText( 10, glutGet( GLUT_WINDOW_HEIGHT ) - 18, VERSION );
    printText( 10, glutGet( GLUT_WINDOW_HEIGHT ) - 32, build_info );

    glutSwapBuffers();
    
    if( frame > 1000 ) frame = 0;
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
    VERSION = "Rolls Royce Motor Cars, " + VERSION;
    getline( ver_file, BUILD_ID );
    getline( ver_file, BUILD_TIME );
    build_info = "Build info: " + BUILD_ID + " @ " + BUILD_TIME;

    // load the Rolls Royce
    rolls.name = "Rolls Royce";
    rolls.setColor( 0, 1, 0 );

    rolls.loadFromPovFile( "vector/rolls_full_n.pov" );
    // move it outside the visible area
    rolls.move( 1000, 0 );

    // load some text
    dcol.R = 1;
    dcol.G = 1;
    dcol.B = 1;
    since1904.loadFromPovFile( "vector/since1904.pov" );

    // animation 1 - the rolls moves into visible area
    a1.type =       1;
    a1.duration = 200;
    a1.repeat   = false;
    a1.q1  = -1000.0f;
    a1.q2  = 0.0f;

    // animation 1 - the rolls becomes red
    a2.type =       4;
    a2.duration = 200;
    a2.repeat   = false;
    a2.q1  = 1.0f;
    a2.q2  = 0.0f;
    a2.q3  = 0.0f;

//    bp1.loadFromPovFile("vector/body.pov");

}

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch( key )
    {
        case 'j':
            my_line.rotate( 5 );
            break;
        case 'k':
            my_line.rotate( -5 );
            break;
        case 'l':
//            shape.move( 5, 0 );
            break;
        case 'h':
//            shape.move( -5, 0);
            break;
        case 27:
            exit(0);
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
    glutCreateWindow( "Rolls Royce Motor Cars" );
    init();

    // callbacks
    glutDisplayFunc( myDisplayFunc );
    glutKeyboardFunc( myKeyboardFunc );
    glutReshapeFunc( myReshape );
    glutVisibilityFunc( visibility );

    glutMainLoop();
    return 0;   // ANSI C requires main to return int.
}
