/* Computer Graphics, Assignment 5, Orthogonal projection
 *
 * Filename ........ ortho.c
 * Description ..... Contains the re-programmed orthogonal projection matrix
 * Date ............ 01.09.2006
 * Created by ...... Jurgen Sturm 
 *
 * Student name ....
 * Student email ... 
 * Collegekaart ....
 * Date ............
 * Comments ........
 *
 *
 * (always fill in these fields before submitting!!)
 */
#include <GL/glut.h>   
#include <GL/gl.h>
#include <GL/glu.h>

 
#define sqr(x) ((x)*(x))

/* ANSI C/ISO C89 does not specify this constant (?) */
#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

void myOrtho(GLdouble left,
             GLdouble right,
             GLdouble bottom,
             GLdouble top,
             GLdouble near,
             GLdouble far)
{
    // secondly, scale the rectangle into a cube
    GLfloat M[16] =
    {
        0.5 * (right - left), 0, 0, 0,
        0, 0.5 * (top - bottom), 0, 0,
        0, 0, 0.5 * (near - far), 0,
        0, 0, 0, 1
    };

    glMultMatrixf(M);

    // first translate to the origin
    GLfloat T[16] =
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        // final column
        -0.5 * (left + right),
        -0.5 * (bottom + top),
        -0.5 * (near + far),
        1
    };

    glMultMatrixf(T);
}
