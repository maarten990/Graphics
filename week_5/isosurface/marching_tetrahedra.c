/* Computer Graphics, Assignment, Volume rendering with cubes/points/isosurface
 *
 * Student name ....
 * Student email ...
 * Collegekaart ....
 * Date ............
 * Comments ........
 *
 * (always fill in these fields before submitting!!)
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "marching_tetrahedra.h"

/* Compute a linearly interpolated position where an isosurface cuts
   an edge between two vertices (p1 and p2), each with their own
   scalar value (v1 and v2) */

static vec3
interpolate_points(unsigned char isovalue, vec3 p1, vec3 p2, unsigned char v1, unsigned char v2)
{

    float d;
    // subtract lowest from biggest value and create new balance
    if (v1 < v2)
    {
        v2 -= v1;
        isovalue -= v1;
        d = isovalue / (float)v2;

        // Apply rate on right position
        return v3_add(v3_multiply(p1,  d), v3_multiply(p2, 1 - d));
    }
    else
    {
        v1 -= v2;
        isovalue -= v2;
        d = isovalue / (float)v1;
        return v3_add(v3_multiply(p1, 1 - d), v3_multiply(p2, d));

    }


}

/* Adds a triangle by interpolating over the given edges (which are expected to
 * contain an intersection).
 * The edges are specified by their vertices (u is the first edge, v the second,
 * etc.)
 */
void interpolate_edges(triangle *triangles, unsigned char isovalue, cell c,
                       int u0, int u1,
                       int v0, int v1,
                       int w0, int w1)
{
    vec3 p1, p2, p3;
    p1 = interpolate_points(isovalue, c.p[u0], c.p[u1], c.value[u0], c.value[u1]);
    p2 = interpolate_points(isovalue, c.p[v0], c.p[v1], c.value[v0], c.value[v1]);
    p3 = interpolate_points(isovalue, c.p[w0], c.p[w1], c.value[w0], c.value[w1]);

    triangles->p[0] = p1;
    triangles->p[1] = p2;
    triangles->p[2] = p3;

    // the surface-normal is the cross-product between 2 of the edges making up
    // the face
    vec3 normal = v3_crossprod(v3_subtract(c.p[u1], c.p[u0]),
                               v3_subtract(c.p[v1], c.p[v0]));

    triangles->n[0] = v3_multiply(v3_add(triangles->n[0], normal), 1.0/3.0);
    triangles->n[1] = v3_multiply(v3_add(triangles->n[1], normal), 1.0/3.0);
    triangles->n[2] = v3_multiply(v3_add(triangles->n[2], normal), 1.0/3.0);
}

/* Using the given iso-value generate triangles for the tetrahedron
   defined by corner vertices v0, v1, v2, v3 of cell c.

   Store the resulting triangles in the "triangles" array.

   Return the number of triangles created (either 0, 1, or 2).

   Note: the output array "triangles" should have space for at least
         2 triangles.
*/
static int
generate_tetrahedron_triangles(triangle *triangles, unsigned char isovalue, cell c, int v0, int v1, int v2, int v3)
{
    unsigned char mask = 0x0;

    /*
     * bitmask: 0000
     * bits in hex: 0x8, 0x4, 0x2, 0x1
     */
    if (c.value[v0] >= isovalue)
        mask |= 0x1;
    if (c.value[v1] >= isovalue)
        mask |= 0x2;
    if (c.value[v2] >= isovalue)
        mask |= 0x4;
    if (c.value[v3] >= isovalue)
        mask |= 0x8;

    switch (mask) {
        // 0001 or 1110
        case 0x1:
        case 0xE:
            interpolate_edges(triangles, isovalue, c, v0, v1, v0, v2, v0, v3);
            return 1;
        // 0010 or 1101
        case 0x2:
        case 0xD:
            interpolate_edges(triangles, isovalue, c, v1, v2, v1, v3, v1, v0);
            return 1;
        // 0100 or 1011
        case 0x4:
        case 0xB:
            interpolate_edges(triangles, isovalue, c, v2, v1, v2, v3, v2, v0);
            return 1;
        // 1000 or 0111
        case 0x8:
        case 0x7:
            interpolate_edges(triangles, isovalue, c, v3, v0, v3, v1, v3, v2);
            return 1;
        // 0011 or 1100
        case 0x3:
        case 0xC:
            interpolate_edges(triangles, isovalue, c, v1, v3, v1, v2, v3, v0);
            interpolate_edges(triangles, isovalue, c, v3, v0, v0, v2, v2, v1);
            return 2;
        // 0101 or 1010
        case 0x5:
        case 0xA:
            interpolate_edges(triangles, isovalue, c, v0, v3, v3, v2, v2, v1);
            interpolate_edges(triangles, isovalue, c, v0, v3, v0, v1, v1, v2);
            return 2;
        // 0110 or 1001
        case 0x6:
        case 0x9:
            interpolate_edges(triangles, isovalue, c, v3, v2, v2, v0, v3, v1);
            interpolate_edges(triangles, isovalue, c, v0, v1, v0, v2, v3, v1);
            return 2;
        // 0000 or 1111
        default:
            return 0;
    }
}


/* Generate triangles for a single cell for the given iso-value. This function
   should produce at most 6 * 2 triangles (for which the "triangles" array should
   have enough space).

   Use calls to generate_tetrahedron_triangles().

   Return the number of triangles produced
*/

int
generate_cell_triangles(triangle *triangles, cell c, unsigned char isovalue)
{
    // keep track of the triangles entered so far to give as offset to the array
    int num_triangles = 0;

    // add each tetrahedron
    num_triangles += generate_tetrahedron_triangles(triangles + num_triangles,
                                                    isovalue, c, 0, 1, 3, 7);
    num_triangles += generate_tetrahedron_triangles(triangles + num_triangles,
                                                    isovalue, c, 0, 2, 6, 7);
    num_triangles += generate_tetrahedron_triangles(triangles + num_triangles,
                                                    isovalue, c, 0, 1, 5, 7);
    num_triangles += generate_tetrahedron_triangles(triangles + num_triangles,
                                                    isovalue, c, 0, 2, 3, 7);
    num_triangles += generate_tetrahedron_triangles(triangles + num_triangles,
                                                    isovalue, c, 0, 4, 5, 7);
    num_triangles += generate_tetrahedron_triangles(triangles + num_triangles,
                                                    isovalue, c, 0, 4, 6, 7);

    return num_triangles;
}
