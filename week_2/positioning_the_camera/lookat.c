/* Computer Graphics, Assignment 4, Positioning the camera
 *
 * Filename ........ lookat.c
 * Description ..... Contains the re-programmed lookAt function
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
#include <math.h>

 
/* ANSI C/ISO C89 does not specify this constant (?) */
#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

// normalizes a 1x3 vector
void normalize(GLdouble *v)
{
    GLdouble x = v[0],
            y = v[1],
            z = v[2];

    GLdouble length = sqrt(x * x + y * y + z * z);
    
    v[0] = x / length;
    v[1] = y / length;
    v[2] = z / length;
}

// performs the cross product
void cross(GLdouble *target, GLdouble *x, GLdouble *y)
{
    target[0] = x[1] * y[2] - y[1] * x[2];
    target[1] = x[2] * y[0] - y[2] * x[0];
    target[2] = x[0] * y[1] - y[0] * x[1];
}

void myLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
              GLdouble centerX, GLdouble centerY, GLdouble centerZ,
              GLdouble upX, GLdouble upY, GLdouble upZ)
{
    // first move the camera to the origin
    glTranslatef(-eyeX, -eyeY, -eyeZ);

    // then perform the coordinate rotation
    GLdouble cz[3] = {eyeX - centerX, eyeY - centerY, eyeZ - centerZ},
             cx[3],
             cy[3],
             up[3] = {upX, upY, upZ};

    normalize(cz);

    cross(cx, cz, up);
    normalize(cx);

    cross(cy, cx, cz);

    GLfloat M[16] =
    {
        cx[0], cy[0], cz[0], 0,
        cx[1], cy[1], cz[1], 0,
        cx[2], cy[2], cz[2], 0,
        0.0, 0.0, 0.0, 1.0
    };

    glMultMatrixf(M);

    // now translate it back
    glTranslatef(eyeX, eyeY, eyeZ);
}
