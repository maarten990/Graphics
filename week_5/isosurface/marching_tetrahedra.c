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
    /* Initially, simply return the midpoint between p1 and p2.
       So no real interpolation is done yet */

    return v3_add(v3_multiply(p1, 0.5), v3_multiply(p2, 0.5));
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
    if (c.value[v0] > isovalue)
        mask |= 0x8;
    if (c.value[v1] > isovalue)
        mask |= 0x4;
    if (c.value[v2] > isovalue)
        mask |= 0x2;
    if (c.value[v3] > isovalue)
        mask |= 0x1;

    printf("%d\n", mask);

    return 0;
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
