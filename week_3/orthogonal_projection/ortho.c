/* Computer Graphics, Assignment 5, Orthogonal projection
 *
 * Filename ........ ortho.c
 * Description ..... Contains the re-programmed orthogonal projection matrix
 * Date ............ 01.09.2006
 * Created by ...... Jurgen Sturm 
 *
 * Student name .... Inge Becht, Maarten de Jonge
 * Student email ... inge.becht91@gmail.com mdejonge1990@gmail.com
 * Collegekaart .... 6093906 6286658
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
             GLdouble far) {

    // Scale it down to a 2x2x2 cube
    GLfloat M[16] =
    {
        2/(right - left), 0.0, 0.0, 0.0,
        0.0, 2/(top -bottom), 0.0, 0.0,
        0.0, 0.0, -2 /(far - near), 0.0,
        0.0, 0.0, 0.0, 1.0
    };


    // translation to the origin
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

    // first translate, then scale
    glMultMatrixf(M);
    glMultMatrixf(T);
}
