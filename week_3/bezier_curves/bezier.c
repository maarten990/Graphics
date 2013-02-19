/* Computer Graphics, Assignment, Bezier curves
 * Filename ........ bezier.c
 * Description ..... Bezier curves
 * Date ............ 22.07.2009
 * Created by ...... Paul Melis
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

#include <math.h>
#include "bezier.h"
#include <stdio.h>

// Returns the factorial of n.
long factorial(int n)
{
    if (n < 2)
        return 1;

    else {
        long fact = 2;
        for (int i = 3; i <=n; ++i) {
            fact *= i;
        }

        return fact;
    }
}

// Returns the value of u as input to the i'th Bernstein polynomial of the n'th
// degree
float bernstein(int i, int n, float u)
{
    // calculate binomial(n, i)
    float n_over_i = (float)factorial(n) / ( factorial(i) * factorial(n - i) );

    return n_over_i * pow(u, i) * pow(1 - u, n - i);
}

/* Given a Bezier curve defined by the 'num_points' control points
 * in 'p' compute the position of the point on the curve for parameter
 * value 'u'.
 *
 * Return the x and y values of the point by setting *x and *y,
 * respectively.
 */

void
evaluate_bezier_curve(float *x, float *y, control_point p[], int num_points, float u)
{
    *x = 0.0;
    *y = 0.0;

    // the result is a summation of Bernstein polynomials over each control
    // point
    for (int i = 0; i < num_points; i++) {
        if(num_points == 2)
        {
            printf("px = %f, pz = %f\n", p[i].x, p[i].y);
            printf("u = %f\n", u);
            printf("i = %f\n", i);
        }
        *x += bernstein(i, num_points-1, u) * p[i].x;
        *y += bernstein(i, num_points-1, u) * p[i].y;
    }
}

/* Draw a Bezier curve defined by the control points in p[], which
 * will contain 'num_points' points.
 *
 * Draw the line segments you compute directly on the screen
 * as a single GL_LINE_STRIP. This is as simple as using
 *
 *      glBegin(GL_LINE_STRIP);
 *      glVertex2f(..., ...);
 *      ...
 *      glEnd();
 *
 * DO NOT SET ANY COLOR!
 *
 * The 'num_segments' parameter determines the "discretization" of the Bezier
 * curve and is the number of straight line segments that should be used
 * to approximate the curve.
 *
 * Call evaluate_bezier_curve() to compute the necessary points on
 * the curve.
 */

void
draw_bezier_curve(int num_segments, control_point p[], int num_points)
{
    float stepsize = (float)1 / num_segments;
    float x = 0,
          y = 0;

    glBegin(GL_LINE_STRIP);

    for (float u = 0; u <= 1.0; u += stepsize) {
        evaluate_bezier_curve(&x, &y, p, num_points, u);
        if(num_points == 2 && num_segments == 2)
           printf("x = %f, z = %f\n", x, y);
        glVertex2f(x, y);
    }

    glEnd();
}

/* Find the intersection of a cubic Bezier curve with the line X=x.
   Return 1 if an intersection was found and place the corresponding y
   value in *y.
   Return 0 if no intersection exists.
*/

int
intersect_cubic_bezier_curve(float *y, control_point p[], float x)
{
    return 0;
}

