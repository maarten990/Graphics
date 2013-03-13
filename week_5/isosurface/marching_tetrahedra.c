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

    float d;
    // Translate so that one of them is zero
    if (v1 < v2)
    {
        v2 -= v1;
        isovalue -= v1;
        d = isovalue / (float)v2;
        return v3_add(v3_multiply(p1,  0.5), v3_multiply(p2,0.5));
    }
    else{
        v1 -= v2;
        isovalue -= v2;
        d = isovalue / (float)v1;
        return v3_add(v3_multiply(p1, 0.5), v3_multiply(p2, 0.5));

    }


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

    // Fill the bit mask
    if(c.value[v0] > isovalue)
    {
        //fprintf(stderr, "in here\n");
        mask |= 0x8;
    }
    if(c.value[v1] > isovalue)
    {
        //fprintf(stderr, "in here\n");
        mask |= 0x4;
    }
    if(c.value[v2] > isovalue)
    {
    //    fprintf(stderr, "in here\n");
        mask |= 0x2;
    }
    if(c.value[v3] > isovalue)
    {
     //   fprintf(stderr, "in here\n");
        mask |= 0x1;
    }

    int t[] = {0x1,0x2,0x4,0x8};
    int t2[] = {v3, v2, v1, v0};

        fprintf(stderr, "mask: %d\n", mask);

    // Check for cases where 1 point is different from the others
    for(int i = 0; i < 4; i++)
    {
        if(mask == t[i] || mask == (0xf - t[i]))
        {
            // fill all elements of triangles p
            int index = 0;
            for(int j = 0; j < 4; j ++)
            {
                if(j == i)
                {
                    continue;
                }
                else
                {

                    // Fill in one of the values for triangles
                    triangles->p[index] = interpolate_points(isovalue, c.p[t2[i]], c.p[t2[j]], c.value[t2[i]], c.value[t2[j]]);
                    triangles->n[index] = c.n[t2[j]];
                    index ++;
                }
            }
            //fprintf(stderr, "found one triangle\n");
            return 1;
        }
    }

    // Check for cases where 2 points are different from the other
    switch(mask)
    {
        case 0x3:
        case 0xc:
            triangles->p[0] = interpolate_points(isovalue, c.p[v3], c.p[v1], c.value[v2], c.value[v1]);
            triangles->n[0] = c.n[v1];
            triangles->p[1] = interpolate_points(isovalue, c.p[v3], c.p[v0], c.value[v3], c.value[v0]);
            triangles->n[1] = c.n[v0];
            triangles->p[2] = interpolate_points(isovalue, c.p[v2], c.p[v1], c.value[v2], c.value[v1]);
            triangles->n[2] = c.n[v1];
            triangles++;
            triangles->p[0] = interpolate_points(isovalue, c.p[v2], c.p[v1], c.value[v2], c.value[v1]);
            triangles->n[0] = c.n[v1];
            triangles->p[1] = interpolate_points(isovalue, c.p[v2], c.p[v0], c.value[v2], c.value[v0]);
            triangles->n[2] = c.n[v0];
            triangles->p[2] = interpolate_points(isovalue, c.p[v3], c.p[v0], c.value[v3], c.value[v0]);
            triangles->n[3] = c.n[v0];
            //fprintf(stderr, "found two triangle\n");
            return 2;
        case 0x5:
        case 0xa:
            triangles->p[0] = interpolate_points(isovalue, c.p[v3], c.p[v2], c.value[v3], c.value[v2]);
            triangles->n[0] = c.n[v2];
            triangles->p[1] = interpolate_points(isovalue, c.p[v3], c.p[v0], c.value[v3], c.value[v0]);
            triangles->n[1] = c.n[v0];
            triangles->p[2] = interpolate_points(isovalue, c.p[v1], c.p[v2], c.value[v1], c.value[v2]);
            triangles->n[2] = c.n[v2];
            triangles++;
            triangles->p[0] = interpolate_points(isovalue, c.p[v1], c.p[v2], c.value[v1], c.value[v2]);
            triangles->n[0] = c.n[v2];
            triangles->p[1] = interpolate_points(isovalue, c.p[v1], c.p[v0], c.value[v1], c.value[v0]);
            triangles->n[1] = c.n[v0];
            triangles->p[2] = interpolate_points(isovalue, c.p[v3], c.p[v2], c.value[v3], c.value[v2]);
            triangles->n[2] = c.n[v2];
            //fprintf(stderr, "found two triangle\n");
            return 2;

        case 0x6: 
        case 0x9:
            triangles->p[0] = interpolate_points(isovalue, c.p[v1], c.p[v3], c.value[v1], c.value[v3]);
            triangles->n[0] = c.n[v3];
            triangles->p[1] = interpolate_points(isovalue, c.p[v1], c.p[v0], c.value[v1], c.value[v0]);
            triangles->n[1] = c.n[v0];
            triangles->p[2] = interpolate_points(isovalue, c.p[v2], c.p[v3], c.value[v2], c.value[v3]);
            triangles->n[2] = c.n[v3];
            triangles++;
            triangles->p[0] = interpolate_points(isovalue, c.p[v1], c.p[v0], c.value[v1], c.value[v2]);
            triangles->n[0] = c.n[v2];
            triangles->p[1] = interpolate_points(isovalue, c.p[v1], c.p[v3], c.value[v1], c.value[v0]);
            triangles->n[1] = c.n[v0];
            triangles->p[2] = interpolate_points(isovalue, c.p[v2], c.p[v0], c.value[v2], c.value[v0]);
            triangles->n[2] = c.n[v0];
            //fprintf(stderr, "found two triangle\n");
            return 2;


    } 
    if(mask > 0 && mask != 15)
    {

    }
    
    // If none of the cases matched all vlaues were the same. Return 0
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
    // Number of triangles
    int num_triangles = 0;

    // Look for triangles in all six possible tetrahedrons in a cell
    num_triangles += generate_tetrahedron_triangles(triangles + num_triangles, isovalue, c, 0, 1, 3, 7);
    num_triangles += generate_tetrahedron_triangles(triangles + num_triangles, isovalue, c, 0, 2, 6, 7);
    num_triangles += generate_tetrahedron_triangles(triangles + num_triangles, isovalue, c, 0, 1, 5, 7);
    num_triangles += generate_tetrahedron_triangles(triangles + num_triangles, isovalue, c, 0, 2, 3, 7);
    num_triangles += generate_tetrahedron_triangles(triangles + num_triangles, isovalue, c, 0, 4, 5, 7);
    num_triangles += generate_tetrahedron_triangles(triangles + num_triangles, isovalue, c, 0, 4, 6, 7);

    return num_triangles;
}

