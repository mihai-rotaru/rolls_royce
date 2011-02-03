#include <GL/glu.h>
#include "globals.h"
#include "xmx_bezier.h"

xmx_BezierPath::xmx_BezierPath( GL_3DPointf& e1, GL_3DPointf& c1, GL_3DPointf& c2, GL_3DPointf& e2 )
{
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

xmx_BezierPath::~xmx_BezierPath()
{
}

void xmx_BezierPath::draw()
{
    if( XMX_SHOW_BEZIER_POINTS )
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
}


void xmx_BezierPath::print()
{
    return /* something */;
}


void xmx_BezierPath::rotate( float )
{
    return /* something */;
}

