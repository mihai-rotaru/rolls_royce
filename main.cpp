#include <GL/glut.h>
#include <GL/glu.h>
#include <iostream>
using namespace std;

#include "globals.h"
#include "xmx_Line.h"
#include "xmx_bezier.h"
xmx_Line my_line( 100, 150, 200, 300 );

// bezier stuff
GL_3DPointf e1( 20, 20, 0 );
GL_3DPointf c1( 20, 100, 0 );
GL_3DPointf c2( 100, 30, 0 );
GL_3DPointf e2( 100, 130, 0 );
xmx_BezierPath bez_path( e1, c1, c2, e2 );

void myDisplayFunc( void )
{
    // clear all pixels
    glRenderMode( GL_RENDER );
    glClear( GL_COLOR_BUFFER_BIT );

    // set line colour red( r=1, g=0,b=0 ).
    glColor3f( 1.0, 0.0, 0.0 );

    // draw a line from point( 100,150 ) to point( 200, 300 )
    my_line.draw();

    // draw the bezier curve !
    bez_path.draw();

    // keep showing( flushing ) line on the screen instead of showing just once.
//    glFlush();
     glutPostRedisplay();
}

void init( void )
{
    // select clearing color
    glClearColor( 0.0, 0.0, 0.0, 0.0 );

    // initialize viewing values
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0,500,0,500 );
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
        case 27:
            exit(0);
            break;
    }
    glutPostRedisplay();
}



int main( int argc, char** argv )
{
    // init
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
    window_width = 500;
    window_height = 500;
    glutInitWindowSize( window_width, window_height );
    glutInitWindowPosition( 100, 100 );
    glutCreateWindow( "Hello" );
    init();

    // callbacks
    glutDisplayFunc( myDisplayFunc );
    glutKeyboardFunc( myKeyboardFunc );

    glutMainLoop();
    return 0;   // ANSI C requires main to return int.
}
