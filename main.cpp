#include <iostream>
#include <fstream>
#include <sstream>
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
string strFPS;

// bezier stuff
//Point e1(  30, 70  );
//Point c1(   0, 270 );
//Point c2( 290, 110 );
//Point e2( 200, 100 );
Point e1( 200,100 );
Point c1( 200,300 );
Point c2( 400,200 );
Point e2( 500,300 );
BezierCurve bez_path( e1, c1, c2, e2 );
Line line( e1, e2 );

// groups - loaded from .pov files
Group rolls;
Group since1904;
Group phony;

// the count of frames rendered so far
GLint frame;
Color text_col(0,0,0);

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
Animation turn_white;
Animation turn_black;

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

void performAnimationColor( Color* c, Animation& a )
{
    if( DEBUG_ANIMATIONS ) cout << "color animation: currentFrame = " << a.currentFrame << " / " << a.duration << endl;

    if( a.type == 4 ) // color
    {
        if( DEBUG_ANIMATIONS )
            cout << "old color: R = " <<c -> R <<" ; G = " <<c -> G <<" ; B = " <<c -> B << endl; 

        c -> R = c -> R + (GLfloat)( a.q1 - c -> R )/( a.duration - a.currentFrame ); 
        c -> G = c -> G + (GLfloat)( a.q2 - c -> G )/( a.duration - a.currentFrame );
        c -> B = c -> B + (GLfloat)( a.q3 - c -> B )/( a.duration - a.currentFrame );

        if( DEBUG_ANIMATIONS )
            cout << "new color: R = " <<c -> R <<" ; G = " <<c -> G <<" ; B = " <<c -> B << endl; 
    }
    else return;

    a.currentFrame++;
    if( a.currentFrame == a.duration )
    {
        if( !a.repeat ) return;
        else a.currentFrame = 0;
    }

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
    if( SHOW_FRAME_NUMBER ) cout<< endl << "FRAME: "<< frame << endl;

    //-------------------------------------------------------------------
    //  BEGIN ANIMATION
    //-------------------------------------------------------------------

    if( frame <= 200 )
    {
        if     ( frame <= 100 ) performAnimationColor( &text_col, turn_white );
        else if( frame >= 170 ) performAnimationColor( &text_col, turn_black );
        glColor3f( text_col.R, text_col.G, text_col.B );
        printBigText( glutGet( GLUT_WINDOW_WIDTH)/2 - 50, glutGet( GLUT_WINDOW_HEIGHT )/2,
                "since 1904...");
    }

    if( frame == 201 ) { text_col.R = 0; text_col.G =0; text_col.B = 0; }
    else if( frame >= 202 && frame <= 400 )
    {
        if  ( frame >= 370 ) performAnimationColor( &text_col, turn_black );
        else performAnimationColor( &text_col, turn_white );
        glColor3f( text_col.R, text_col.G, text_col.B );
        printBigText( glutGet( GLUT_WINDOW_WIDTH)/2 - 150, glutGet( GLUT_WINDOW_HEIGHT )/2,
                "we've been designing and building...");
    }
    
    if( frame == 401 ) { text_col.R = 0; text_col.G =0; text_col.B = 0; }
    else if( frame >= 402 && frame <= 550 )
    {
        if( frame >= 520 ) performAnimationColor( &text_col, turn_black );
        else performAnimationColor( &text_col, turn_white );
        glColor3f( text_col.R, text_col.G, text_col.B );
        printBigText( glutGet( GLUT_WINDOW_WIDTH)/2 - 50, glutGet( GLUT_WINDOW_HEIGHT )/2,
                "perfection");
    }

    if( frame == 551 ) { text_col.R = 0; text_col.G =0; text_col.B = 0; }
    else if( frame >= 552 && frame <= 700 )
    {
        if( frame >= 670 ) performAnimationColor( &text_col, turn_black );
        else performAnimationColor( &text_col, turn_white );
        glColor3f( text_col.R, text_col.G, text_col.B );
        printBigText( glutGet( GLUT_WINDOW_WIDTH)/2 - 190, glutGet( GLUT_WINDOW_HEIGHT )/2,
                "meet the Rolls Royce Ghost, our latest creation");
    }

    if( frame >= 650 )
    {
        performAnimation( &rolls, a1 );
        performAnimation( &rolls, a2 );
    } 
    
    rolls.draw();
//    since1904.draw();
    
    bez_path.rotate(1);
    bez_path.draw();
//    line.rotate2(1);
//    line.draw();

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
    VERSION = "Rolls Royce Motor Cars, " + VERSION;
    getline( ver_file, BUILD_ID );
    getline( ver_file, BUILD_TIME );
    build_info = "Build info: " + BUILD_ID + " @ " + BUILD_TIME;

    // display FPS
    stringstream ss;
    ss << FPS;
    strFPS = "FPS: " + ss.str();

    // load the Rolls Royce
    rolls.name = "Rolls Royce";
    rolls.loadFromPovFile( "vector/rolls_full_n.pov" );
    rolls.setColor( 0, 1, 0 );
    // move it outside the visible area
    rolls.move( 1500, 0 );

    // load some text
    dcol.R = 1;
    dcol.G = 1;
    dcol.B = 1;
    since1904.loadFromPovFile( "vector/since1904.pov" );

    // animation 1 - the rolls moves into visible area
    a1.type =         1;
    a1.duration =   200;
    a1.repeat   = false;
    a1.q1  =   -1500.0f;
    a1.q2  =       0.0f;

    // animation 1 - the rolls becomes red
    a2.type =         4;
    a2.duration =   200;
    a2.repeat   = false;
    a2.q1  =       1.0f;
    a2.q2  =       0.0f;
    a2.q3  =       0.0f;

    // turns a color to white
    turn_white.type = 4;
    turn_white.duration = 50;
    turn_white.repeat = true;
    turn_white.q1 =        1;
    turn_white.q2 =        1;
    turn_white.q3 =        1;

    // turns a color to black
    turn_black.type = 4;
    turn_black.duration = 20;
    turn_black.repeat = true;
    turn_black.q1 =        0;
    turn_black.q2 =        0;
    turn_black.q3 =        0;
    
}

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch( key )
    {
        case 'j':
            bez_path.move(  0, -5 );
            break;
        case 'k':
            bez_path.move(  0,  5 );
            break;
        case 'l':
            bez_path.move(  5,  0 );
//            shape.move( 5, 0 );
            break;
        case 'h':
            bez_path.move( -5,  0 );
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
