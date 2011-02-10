#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

#include <GL/glu.h>

#include "globals.h"
#include "Point.h"
#include "BezierCurve.h"

namespace xmx {

BezierCurve::BezierCurve( Point e1, Point c1, Point c2, Point e2 )
{
    if( DEBUG_CTOR_DTOR ) cout<<"ctor for BezierCurve @ " << this << endl;
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

    calculateBoundingBox();
    if( DEBUG_INIT_BEZIER_BOUNDING_BOX ) printBoundingBox();
}

BezierCurve::BezierCurve( 
        GLfloat e1_x, GLfloat e1_y,
        GLfloat c1_x, GLfloat c1_y,
        GLfloat c2_x, GLfloat c2_y,
        GLfloat e2_x, GLfloat e2_y )
{
    if( DEBUG_CTOR_DTOR ) cout<<"ctor for BezierCurve @ " << this << endl;
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

    calculateBoundingBox();

    if( DEBUG_INIT_BEZIER_BOUNDING_BOX ) printBoundingBox();
}


BezierCurve::~BezierCurve()
{
    if( DEBUG_CTOR_DTOR ) cout<<"dtor for BezierCurve @ " << this << endl;
}

void BezierCurve::printBoundingBox()
{
    cout<< "maxX: " << maxX << endl;
    cout<< "maxY: " << maxY << endl;
    cout<< "minX: " << minX << endl;
    cout<< "minY: " << minY << endl;
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

	glBegin( GL_LINE_STRIP );
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

void BezierCurve::calculateBoundingBox()
{
    bool DBBB = DEBUG_BEZIER_BOUNDING_BOX;
    if( DBBB ) cout<< "entering drawBoundingBox() for BezierCurve @ " << this << endl; 

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

    Point P0 = getP0();
    Point P1 = getP1();
    Point P2 = getP2();
    Point P3 = getP3();

    if( DBBB )cout<<"calculating coeficients..."<<endl;
    Point a = P3 - 3*P2 + 3*P1 - P0;     if( DBBB )a.print("a");
    Point b = 3*P2 - 6*P1 + 3*P0;        if( DBBB )b.print("b");
    Point c = 3*P1 - 3*P0;               if( DBBB )c.print("c");
    Point d = P0;                        if( DBBB )d.print("d");
    
    if( DBBB )cout<<"for the derived function: "<<endl;
    a = 3*a; if( DBBB )a.print("a");
    b = 2*b; if( DBBB )b.print("b");

    // Discriminant
    Point D = b*b - 4*( a * c );
    if( DBBB ) { cout<<"Discriminant: "; D.print("D"); }

    // solutions
    if( DBBB )cout<< "calcutating solutions..."<<endl;
    Point s1 = ( -1 * b + psqrt( b * b - 4 * a * c ))/ ( 2 * a  ); if( DBBB )s1.print("s1");
    Point s2 = ( -1 * b - psqrt( b * b - 4 * a * c ))/ ( 2 * a  ); if( DBBB )s2.print("s2");

    // put solutions in an array
    GLfloat solutions[4]; 
    solutions[0] = s1.x;
    solutions[1] = s2.x;
    solutions[2] = s1.y; 
    solutions[3] = s2.y;

    // change a and b back to values for the original function, not the derivative
    a = a/3; if( DBBB )a.print("a");
    b = b/2; if( DBBB )b.print("b");

    // determine max and min x/y for end points
    maxX = P0.x > P3.x ? P0.x : P3.x;
    maxY = P0.y > P3.y ? P0.y : P3.y;
    minX = P0.x < P3.x ? P0.x : P3.x;
    minY = P0.y < P3.y ? P0.y : P3.y;

    // calculate the Bezier's equation for each solution
    for( int i=0; i<=3; i++ )
        if( solutions[i] < 1 ) // Beziers are only defined for [0,1]
        {
            GLfloat x = solveFor( solutions[i], a.x, b.x, c.x, d.x ); if( DBBB )cout<< "s-x" <<i+1<<": "<< x << endl;
            GLfloat y = solveFor( solutions[i], a.y, b.y, c.y, d.y ); if( DBBB )cout<< "s-y" <<i+1<<": "<< y << endl;

            // new min or max ?
            maxX = x > maxX ? x : maxX;
            maxY = y > maxY ? y : maxY;
            minX = x < minX ? x : minX;
            minY = y < minY ? y : minY;
        }

    // find min and max for each solution
}

void BezierCurve::drawBoundingBox()
{
    glBegin( GL_LINES );
        glVertex2f( minX, minY );
        glVertex2f( maxX, minY );

        glVertex2f( maxX, minY );
        glVertex2f( maxX, maxY );
        
        glVertex2f( maxX, maxY );
        glVertex2f( minX, maxY );
        
        glVertex2f( minX, maxY );
        glVertex2f( minX, minY );
    glEnd();
}

void BezierCurve::rotate( GLfloat )
{
    return /* something */;
}

void BezierCurve::rotate( GLfloat, GLfloat, GLfloat )
{
    return /* something */;
}

GLfloat BezierCurve::distanceTo( GLfloat, GLfloat )
{
    return -1;
}

void BezierCurve::move( GLfloat x_dir, GLfloat y_dir )
{
    if( DEBUG_TRANSF_MOVE ) cout << "moving BezierCurve @ " << this << endl;
    for( int i=0; i<=3; i++ )
    {
        points[i][0] += x_dir;
        points[i][1] += y_dir;
    }

    calculateBoundingBox();
}

void BezierCurve::scale( GLfloat, GLfloat )
{
    return /* something */;
}

} // namespace xmx
