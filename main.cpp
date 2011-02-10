#include <iostream>
using namespace std;

#include <GL/glut.h>
#include <GL/glu.h>

#include "globals.h"
#include "Point.h"
#include "Line.h"
#include "BezierCurve.h"
#include "BezierPath.h"
#include "Shape.h"
using namespace xmx;

Line my_line( 100, 150, 200, 300 );

// bezier stuff
Point e1(  30, 70  );
Point c1(   0, 270 );
Point c2( 290, 110 );
Point e2( 200, 100 );
BezierCurve bez_path( e1, c1, c2, e2 );
BezierPath bp1;
Shape shape;

void myDisplayFunc( void )
{
    // clear all pixels
    glRenderMode( GL_RENDER );
    glClear( GL_COLOR_BUFFER_BIT );

    // set line colour red( r=1, g=0,b=0 ).
    glColor3f( 1.0, 0.0, 0.0 );

    // draw a line from point( 100,150 ) to point( 200, 300 )
    my_line.draw();
//    my_line.rotate( 20 );

    // draw the bezier curve !
//    bez_path.draw();
//    bez_path2.draw();
//    bp1.draw();
    shape.draw();

    // keep showing( flushing ) line on the screen instead of showing just once.
    glFlush();
    glutPostRedisplay();
}

void init( void )
{
    // select clearing color
    glClearColor( 0.0, 0.0, 0.0, 0.0 );

    // initialize viewing values
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0,640,0,480 );

//    bp1.addCurve( e1.x, e1.y, c1.x, c1.y, c2.x, c2.y, e2.x, e2.y );
//    bp1.addCurve( 100,100,100,200,200,20,100,300 );
//    shape.addBezierCurve( 100,100,100,200,200,20,100,300 );
    shape = loadFromPovFile( "vector/body.pov");

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
            shape.move( 5, 0 );
            break;
        case 'h':
            shape.move( -5, 0);
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

int main( int argc, char** argv )
{
    // init
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
    window_width = 640;
    window_height = 480;
    glutInitWindowSize( window_width, window_height );
    glutInitWindowPosition( 100, 100 );
    glutCreateWindow( "Hello" );
    init();

    // callbacks
    glutDisplayFunc( myDisplayFunc );
    glutKeyboardFunc( myKeyboardFunc );
    glutReshapeFunc( myReshape );

    glutMainLoop();
    return 0;   // ANSI C requires main to return int.
}
