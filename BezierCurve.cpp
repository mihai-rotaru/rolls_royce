#include <GL/glu.h>
#include "globals.h"
#include "Point.h"
#include "BezierCurve.h"

#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

bool temp = false;

namespace xmx {

BezierCurve::BezierCurve( Point e1, Point c1, Point c2, Point e2 )
{
    cout<<"creating Bezier curve @ "<<this<<endl;
    // first end point
    points[ END_PT_1 ][0] = e1.x;
    points[ END_PT_1 ][1] = e1.y;
    points[ END_PT_1 ][2] = 0.0f; // 2D, so 'z' coord. is always 0

    // first control potint
    points[ CTRL_PT_1 ][0] = c1.x;
    points[ CTRL_PT_1 ][1] = c1.y;
    points[ CTRL_PT_1 ][2] = 0.0f;
    
    // second control potint
    points[ CTRL_PT_2 ][0] = c2.x;
    points[ CTRL_PT_2 ][1] = c2.y;
    points[ CTRL_PT_2 ][2] = 0.0f;

    // second end point
    points[ END_PT_2 ][0] = e2.x;
    points[ END_PT_2 ][1] = e2.y;
    points[ END_PT_2 ][2] = 0.0f; // 2D, so 'z' coord. is always 0
}

BezierCurve::BezierCurve( 
        GLfloat e1_x, GLfloat e1_y,
        GLfloat c1_x, GLfloat c1_y,
        GLfloat c2_x, GLfloat c2_y,
        GLfloat e2_x, GLfloat e2_y )
{
    cout<<"creating Bezier curve @ "<<this<<endl;
    // first end point
    points[ END_PT_1 ][0] = e1_x;
    points[ END_PT_1 ][1] = e1_y;
    points[ END_PT_1 ][2] = 0.0f; // 2D, so 'z' coord. is always 0

    // first control potint
    points[ CTRL_PT_1 ][0] = c1_x;
    points[ CTRL_PT_1 ][1] = c1_y;
    points[ CTRL_PT_1 ][2] = 0.0f;
    
    // second control potint
    points[ CTRL_PT_2 ][0] = c2_x;
    points[ CTRL_PT_2 ][1] = c2_y;
    points[ CTRL_PT_2 ][2] = 0.0f;

    // second end point
    points[ END_PT_2 ][0] = e2_x;
    points[ END_PT_2 ][1] = e2_y;
    points[ END_PT_2 ][2] = 0.0f; // 2D, so 'z' coord. is always 0
}


BezierCurve::~BezierCurve()
{
    cout<<"destructor for BezierCurve @ "<< this << endl;
}

void BezierCurve::draw()
{
    if( SHOW_BEZIER_POINTS )
    {
        glPointSize( 5.0f );
        glBegin( GL_POINTS );
            glVertex2fv( points[ END_PT_1  ] );
            glVertex2fv( points[ CTRL_PT_1 ] );
            glVertex2fv( points[ CTRL_PT_2 ] );
            glVertex2fv( points[ END_PT_2  ] );
        glEnd();
    }

    glMap1f( GL_MAP1_VERTEX_3,
            0.0f,
            100.0f,
            3,
            4, // number of control points
            &points[0][0]
           );


    glEnable( GL_MAP1_VERTEX_3 );

	glBegin(GL_LINE_STRIP);
		for( int i = 0; i <= 100; i++ )
			{
			// Evaluate the curve at this point
			glEvalCoord1f( (GLfloat)i ); 
            }
    glEnd();

    if( SHOW_BEZIER_BOUNDING_BOX ) drawBoundingBox();
}

void BezierCurve::print()
{
    cout<<"BezierCurve, at: "<<this<<endl;
    nice_cout( "EP1: x:", points[ END_PT_1  ][0]);
    nice_cout( "EP1: y:", points[ END_PT_1  ][1]);
    nice_cout( "CP1: x:", points[ CTRL_PT_1 ][0]);
    nice_cout( "CP1: y:", points[ CTRL_PT_1 ][1]);
    nice_cout( "CP2: x:", points[ CTRL_PT_2 ][0]);
    nice_cout( "CP2: y:", points[ CTRL_PT_2 ][1]);
    nice_cout( "EP2: x:", points[ END_PT_2  ][0]);
    nice_cout( "EP2: y:", points[ END_PT_2  ][1]);
    cout<<endl;
}

void BezierCurve::print( string str )
{
    cout<<str<<endl;
    print();
}

// solves the curve's equation for x, a,b,c,d
GLfloat solveFor( GLfloat x, GLfloat a, GLfloat b, GLfloat c, GLfloat d )
{
    return( a*x*x*x + b*x*x + c*x + d );
}

void BezierCurve::drawBoundingBox()
{
    if( !temp )
    {
    temp = true;
    cout<< "entering drawBoundingBox() for BezierCurve @ " << this << endl; 

    /*=====================================================================   
    |                                                                     |
    |    P0 - first end point                                             |
    |    P1 - first control point                                         |
    |    P2 - second control point                                        |
    |    P3 - second end point                                            |
    |                                                                     |
    |    Cubic Bezier ecuation:                                           |
    |    f(x) = ax^3 + bx^2 + cx + d                                      |
    |      a = P3 - 3P2 + 3P1 -  P0                                       |
    |      b =      3P2 - 6P1 + 3P0                                       |
    |      c =            3P1 - 3P0                                       |
    |      d =                   P0                                       |
    |                                                                     |
    |    Derivative:                                                      |
    |    f'(x) = 3at^2 + 2bt + c                                          |
    |                                                                     |
    |    By solving f'(x) = 0, we get local extremes                      |
    |    replacing a-d in the derived ecuation:                           |
    |                                                                     |
    |    3( P3 - 3P2 + 3P1 - P0 )x^2 + 2( 3P2 - 6P1 + 3P0 )x + 3P1- 3P0   |
    |                                                                     |
    |    Discriminant:                                                    |
    |    D = b^2 - 4ac                                                    |
    |                                                                     |
    \====================================================================*/

    Point P0( points[ END_PT_1 ][0], points[ END_PT_1 ][1], 0.0f ); 
    Point P1( points[ CTRL_PT_1][0], points[ CTRL_PT_1][1], 0.0f ); 
    Point P2( points[ CTRL_PT_2][0], points[ CTRL_PT_2][1], 0.0f ); 
    Point P3( points[ END_PT_2 ][0], points[ END_PT_2 ][1], 0.0f ); 

    P0.print();
    P1.print();
    P2.print();
    P3.print();

    Point a = P3 - 3*P2 + 3*P1 - P0;     a.print("a");
    Point b = 3*P2 - 6*P1 + 3*P0;        b.print("b");
    Point c = 3*P1 - 3*P0;               c.print("c");
    Point d = P0;                        d.print("d");
    
    a = 3*a; a.print("a");
    b = 2*b; b.print("b");

    // Determinant
    Point D = b*b - 4*( a * c );
    D.print("D");

    // solutions
    Point s1 = ( -1 * b + psqrt( b * b - 4 * a * c ))/ ( 2 * a  ); s1.print("s1");
    Point s2 = ( -1 * b - psqrt( b * b - 4 * a * c ))/ ( 2 * a  ); s2.print("s2");

    GLfloat sx1 = s1.x;
    GLfloat sx2 = s2.x;
    GLfloat sy1 = s1.y;
    GLfloat sy2 = s2.y;

    cout<<"=================="<<endl;
    cout<<"sx1 = "<<sx1<<endl;
    cout<<"sx2 = "<<sx2<<endl;
    cout<<"sy1 = "<<sy1<<endl;
    cout<<"sy2 = "<<sy2<<endl;
    cout<<"=================="<<endl;

    // change a and b back to values for the original function, not the derivative
    a = a/3; a.print("a");
    b = b/2; b.print("b");

    if ( sx1 < 1 )
    { 
        GLfloat x1 = solveFor( sx1, a.x, b.x, c.x, d.x ); cout<< "s-x1: " << x1 << endl; 
        GLfloat y1 = solveFor( sx1, a.y, b.y, c.y, d.y ); cout<< "s-y1: " << y1 << endl; 
    }

    if ( sx2 < 1 )
    { 
        GLfloat x2 = solveFor( sx2, a.x, b.x, c.x, d.x ); cout<< "s-x2: " << x2 << endl; 
        GLfloat y2 = solveFor( sx2, a.y, b.y, c.y, d.y ); cout<< "s-y2: " << y2 << endl; 
    }
    
    if ( sy1 < 1 )
    { 
        GLfloat x3 = solveFor( sy1, a.x, b.x, c.x, d.x ); cout<< "s-x3: " << x3 << endl; 
        GLfloat y3 = solveFor( sy1, a.y, b.y, c.y, d.y ); cout<< "s-y3: " << y3 << endl; 
    }                           
                                
    if ( sy2 < 1 )              
    {                           
        GLfloat x4 = solveFor( sy2, a.x, b.x, c.x, d.x ); cout<< "s-x4: " << x4 << endl; 
        GLfloat y4 = solveFor( sy2, a.y, b.y, c.y, d.y ); cout<< "s-y4: " << y4 << endl; 
    }
    // find min and max for each solution
    
    }
}


void BezierCurve::rotate( GLfloat )
{
    return /* something */;
}

void BezierCurve::rotate( GLfloat, GLfloat, GLfloat )
{
    return /* something */;
}

GLfloat xmx::BezierCurve::distanceTo( GLfloat, GLfloat )
{
    return -1;
}

GLfloat xmx::BezierCurve::getMaxX()
{
    return -1;
}


GLfloat xmx::BezierCurve::getMaxY()
{
    return -1;
}


GLfloat xmx::BezierCurve::getMinX()
{
    return -1;
}


GLfloat xmx::BezierCurve::getMinY()
{
    return -1;
}


void xmx::BezierCurve::move( GLfloat, GLfloat )
{
    return /* something */;
}

void xmx::BezierCurve::scale( GLfloat, GLfloat )
{
    return /* something */;
}

} // namespace xmx
