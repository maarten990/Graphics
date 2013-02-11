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
#include <string.h>
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

// normalizes a 1x3 vector
void normalize(GLfloat *v)
{
    GLfloat x = v[0],
            y = v[1],
            z = v[2];

    GLfloat length = sqrt(x * x + y * y + z * z);
    
    v[0] = x / length;
    v[1] = y / length;
    v[2] = z / length;
}

void cross(GLfloat *target, GLfloat *x, GLfloat *y)
{
    target[0] = x[1] * y[2] - y[1] * x[2];
    target[1] = x[2] * y[0] - y[2] * x[0];
    target[2] = x[0] * y[1] - y[0] * x[1];
}

void myRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat u[3], v[3], w[3], t[3];

    //
    // 1. Create the orthonormal basis
    //

    // Store the incoming rotation axis in w and normalize w
    w[0] = x;
    w[1] = y;
    w[2] = z;
    normalize(w);

    // Compute the value of t, based on w
    int smallest = 0;
    for (int i = 1; i < 3; ++i) {
        if (w[i] < w[smallest]) {
            smallest = i;
        }
    }

    memcpy(t, w, 3);
    t[smallest] = 1;

    // Compute u = t x w
    cross(u, t, w);

    // Normalize u
    normalize(u);

    // Compute v = w x u
    cross(v, w, u);

    // At this point u, v and w should form an orthonormal basis.
    // If your routine does not seem to work correctly it might be
    // a good idea to the check the vector values.
    /*
    printf("u . v: %f\n", u[0] * v[0] + u[1] * v[1] + u[2] * v[2]);
    printf("u . w: %f\n", u[0] * w[0] + u[1] * w[1] + u[2] * w[2]);
    printf("v . w: %f\n", v[0] * w[0] + v[1] * w[1] + v[2] * w[2]);
    printf("\n");
    */

    //
    // 2. Set up the three matrices making up the rotation
    //

    // Specify matrix A

    GLfloat A[16] =
    {
        u[0], u[1], u[2], 0.0,
        v[0], v[1], v[2], 0.0,
        w[0], w[1], w[2], 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    // Convert 'angle' to radians
    angle *= M_PI / 180.0;

    // Specify matrix B
    GLfloat c = cos(angle);
    GLfloat s = sin(angle);

    GLfloat B[16] =
    {
        c, s, 0.0, 0.0,
        -1 * s, c, 0.0, 0.0,
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

