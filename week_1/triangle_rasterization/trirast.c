/* Computer Graphics assignment, Triangle Rasterization
 * Filename ........ trirast.c
 * Description ..... Implements triangle rasterization
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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "types.h"

float line(float x0, float y0, float x1, float y1,
           float px, float py);

float lowest(float x, float y, float z);
float highest(float x, float y, float z);

/*
 * Returns the value of implicit formula for a line through (x0, y0) and (x1, y1),
 * given the point (px, py) as input.
 */
float line(float x0, float y0, float x1, float y1,
           float px, float py)
{
    return (y0 - y1) * px + (x1 - x0) * py + x0 * y1 - x1 * y0;
}

/*
 * Returns the lowest of 3 floats.
 */
float lowest(float x, float y, float z)
{
    float l = x;
    
    if (y < l) {
        l= y;
    }

    if (z < l) {
        l = z;
    }

    return l;
}

/*
 * Returns the highest of 3 floats.
 */
float highest(float x, float y, float z)
{
    float h = x;
    
    if (y > h) {
        h = y;
    }

    if (z > h) {
        h = z;
    }

    return h;
}

/*
 * Rasterize a single triangle.
 * The triangle is specified by its corner coordinates
 * (x0,y0), (x1,y1) and (x2,y2).
 * The triangle is drawn in color (r,g,b).
 */

void
draw_triangle(float x0, float y0, float x1, float y1, float x2, float y2,
    byte r, byte g, byte b)
{
    float xmin = lowest(x0, x1, x2);
    float xmax = highest(x0, x1, x2);

    float ymin = lowest(y0, y1, y2);
    float ymax = highest(y0, y1, y2);

    float alpha, beta, gamma;

    // loop through each pixel in the triangle bounding box
    for (float x = xmin; x < xmax; ++x) {
        for (float y = ymin; y < ymax; ++y) {
            alpha = line(x1, y1, x2, y2, x, y) / line(x1, y1, x2, y2, x0, y0);
            beta = line(x2, y2, x0, y0, x, y) / line(x2, y2, x0, y0, x1, y1);
            gamma = line(x0, y0, x1, y1, x, y) / line(x0, y0, x1, y1, x2, y2);

            if ((alpha >= 0) && (beta >= 0) && (gamma >= 0)) {
                PutPixel(x, y, r, g, b);
            }
        }
    }
}

void
draw_triangle_optimized(float x0, float y0, float x1, float y1, float x2, float y2,
    byte r, byte g, byte b)
{
    float xmin = lowest(x0, x1, x2);
    float xmax = highest(x0, x1, x2);

    float ymin = lowest(y0, y1, y2);
    float ymax = highest(y0, y1, y2);

    float alpha, beta, gamma,
          pre_loop_alpha,
          pre_loop_beta,
          pre_loop_gamma;

    // initial, pre-loop values
    float alpha_numerator = line(x1, y1, x2, y2, x0, y0);
    float beta_numerator  = line(x2, y2, x0, y0, x1, y1);
    float gamma_numerator = line(x0, y0, x1, y1, x2, y2);

    alpha = line(x1, y1, x2, y2, xmin - 1, ymin - 1) / alpha_numerator;
    beta  = line(x2, y2, x0, y0, xmin - 1, ymin - 1) / beta_numerator;
    gamma = line(x0, y0, x1, y1, xmin - 1, ymin - 1) / gamma_numerator;
            
    // loop through each pixel in the triangle bounding box
    for (float x = xmin; x < xmax; ++x) {
        // update the x-variable componenents
        alpha += (y1 - y2) / alpha_numerator;
        beta += (y2 - y0) / beta_numerator;
        gamma += (y0 - y1) / gamma_numerator;

        // save the parameter values before entering the y-loop
        pre_loop_alpha = alpha;
        pre_loop_beta  = beta;
        pre_loop_gamma = gamma;

        for (float y = ymin; y < ymax; ++y) {
            // update the barycentric coordinates
            alpha += (x2 - x1) / alpha_numerator;
            beta += (x0 - x2) / beta_numerator;
            gamma += (x1 - x0) / gamma_numerator;

            if ((alpha >= 0) && (beta >= 0) && (gamma >= 0)) {
                PutPixel(x, y, r, g, b);
            }
        }

        alpha = pre_loop_alpha;
        beta = pre_loop_beta;
        gamma = pre_loop_gamma;
    }
}
