#ifndef MARCHING_TETRAHEDRA_H
#define MARCHING_TETRAHEDRA_H

#include "v3math.h"
#include "volume.h"

typedef struct
{
    vec3    p[3];
    vec3    n[3];
}
triangle;

int generate_cell_triangles(triangle *triangles, cell c, unsigned char isovalue);
void interpolate_edges(triangle *triangles, unsigned char isovalue, cell c,
                       int u0, int u1,
                       int v0, int v1,
                       int w0, int w1);

#endif
