#ifndef BEZIER_H
#define BEZIER_H

#include <GL/gl.h>

// A single control point of a Bezier curve
typedef struct control_point
{
    float x;
    float y;
}
control_point;

void    evaluate_bezier_curve(float *x, float *y, control_point p[], int num_points, float u);
void    draw_bezier_curve(int num_segments, control_point p[], int num_points);
int     intersect_cubic_bezier_curve(float *y, control_point p[], float x);
long    factorial(int n);
float   bernstein(int i, int n, float u);
int     close_enough(float x, float target, float tolerance);
int     inside_bounding_box(float x, control_point p[], int num_points);

#endif
