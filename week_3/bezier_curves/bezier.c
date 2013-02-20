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

#define INTERSECT_TOLERANCE 0.001

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
        glVertex2f(x, y);
    }

    glEnd();
}

// Returns smallest x value
float min_x(control_point p[], int num_points)
{
    float smallest = p[0].x;
    for(int i = 1; i < num_points; i ++)
    {
        if(p[i].x < smallest)
        {
            smallest = p[i].x;
        }
    }
    return smallest;
}

// Returns largest x value
float max_x(control_point p[], int num_points)
{
    float biggest = p[0].x;
    for(int i = 1; i < num_points; i ++)
    {
        if(p[i].x > biggest)
        {
            biggest = p[i].x;
        }

    }
    return biggest;
}

/* Returns >0 if the two floats are within the tolerance of eachother,
 * otherwise 0. */
int
close_enough(float x, float target, float tolerance)
{
    return ((x < target + tolerance) && (x > target - tolerance)) ? 1 : 0;
}

/* Find the intersection of a cubic Bezier curve with the line X=x.
   Return 1 if an intersection was found and place the corresponding y
   value in *y.
   Return 0 if no intersection exists.
*/
int
intersect_cubic_bezier_curve(float *y, control_point p[], float x)
{
    int num_points = 4;
      // check if x is within convex hull
      float minx = min_x(p, num_points);
      float maxx = max_x(p, num_points);


      // if in first or last set of control points
      if((minx > 10 && maxx <= 20) || (minx >= 0 && maxx < 5))
      {
          // If not inside return false
          if (x < minx || x > maxx)
          {
              //*y = 0;
              return 0;
          }
      }

    /* u represents the position along the curve (interval [0, 1])
     * cx and cy hold the x and y coordinates corresponding to a value of u
     */
    float u  = 0,
          cx = 0,
          cy = 0;

    // because the curves are restricted to act as functions, we can apply
    // binary search
    float min = 0;
    float max = 1;

    // loop until the max and min values get too close for our liking
    while ( !close_enough(max, min, INTERSECT_TOLERANCE) ) {
        u = (max + min) / 2;
        evaluate_bezier_curve(&cx, &cy, p, num_points, u);

        if (close_enough(cx, x, INTERSECT_TOLERANCE)) {
            *y = cy;
            return 1;
        }

        if (cx > x)
            max = u;
        else
            min = u;
    }

    return 0;
}

