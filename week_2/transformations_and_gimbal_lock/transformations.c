/* Computer Graphics, Assignment, Translations, Rotations and Scaling
 *
 * Filename ........ transformations.c
 * Description ..... Contains the re-programmed translation, rotation and scaling functions
 * Student name ....
 * Student email ...
 * Collegekaart ....
 * Date ............
 * Comments ........
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include "transformations.h"

/* ANSI C/ISO C89 does not specify this constant (?) */
#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

void myScalef(GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat M[16] =
    {
        x, 0.0, 0.0, 0.0,
        0.0, y, 0.0, 0.0,
        0.0, 0.0, z, 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    glMultMatrixf(M);
}


void myTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat M[16] =
    {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        x, y, z, 1.0
    };

    glMultMatrixf(M);
}


// Returns cross product of two input vectors to last vector
void crossProduct(GLfloat v1[], GLfloat v2[], GLfloat v3[])
{

    v3[0] = v1[1] * v2[2] - v1[2] * v2[1];
    v3[1] = v1[2] * v2[0] - v1[0] * v2[2];
    v3[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

void myRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat u[3], v[3], w[3], t[3];

    //
    // 1. Create the orthonormal basis
    //
    
    // Store the incoming rotation axis in w and normalize w
    GLfloat length_a;
    length_a = sqrt(x * x + y * y + z * z );
    w[0] = x / length_a;
    w[1] = y / length_a;
    w[2] = z / length_a;


    // Compute the value of t, based on w
    if(w[0] <= w[1] && w[0] <= w[2])
    {
        t[0] = 1;
        t[1] = w[1];
        t[2] = w[2];
    }
    if(w[1] <= w[2] && w[1] <= w[0])
    {
        t[1] = 1;
        t[0] = w[0];
        t[2] = w[2];
    }
    if(w[2] <= w[1] && w[2] <= w[0])
    {
        t[2] = 1;
        t[1] = w[1];
        t[0] = w[0];
    }


    // Compute u = t x w
    crossProduct(t, w, u);
  


    // Normalize u
    GLfloat length_u =sqrt(u[0] * u[0] + u[1] * u[1] + u[2] * u[2]);
    u[0] = u[0] / length_u;
    u[1] = u[1] / length_u;
    u[2] = u[2] / length_u;

    // Compute v = w x u
    crossProduct(w, u, v);

    // At this point u, v and w should form an orthonormal basis.
    // If your routine does not seem to work correctly it might be
    // a good idea to the check the vector values.

    //
    // 2. Set up the three matrices making up the rotation
    //

    // Specify matrix A

    angle = angle * (M_PI/180);
    GLfloat A[16] =
    {
        u[0], u[1], u[2], 0.0,
        v[0], v[1], v[2], 0.0,
        w[0], w[1], w[2], 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    // Convert 'angle' to radians

    // Specify matrix B

    GLfloat B[16] =
    {
        cos(angle), sin(angle), 0.0, 0.0,
        -sin(angle), cos(angle), 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    // Specify matrix C

    GLfloat C[16] =
    {
        u[0], v[0], w[0], 0.0,
        u[1], v[1], w[1], 0.0,
        u[2], v[2], w[2], 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    //
    // 3. Apply the matrices to get the combined rotation
    //
 
    glMultMatrixf(A);
    glMultMatrixf(B);
    glMultMatrixf(C);
}

