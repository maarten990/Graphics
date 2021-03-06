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

#include <stdio.h>
#include <stdlib.h>

#include "volume.h"

/* The voxels of the volume dataset, stored as a one-dimensional array */
unsigned char   *volume;

/* The dimensions of the volume dataset */
int     nx, ny, nz;

/* The size of a voxel */
float   sizex, sizey, sizez;

/* the i, j and k offsets in order as used for the cells, relative to their
 * root-voxel */
vec3 CELL_OFFSETS[8];

/* Utility function to convert the index of a voxel
   into an index in the volume array above */
int
voxel2idx(int i, int j, int k)
{
    return (k*ny + j)*nx + i;
}

void init_cell_offsets()
{
    CELL_OFFSETS[0] = v3_create(0, 0, 0);
    CELL_OFFSETS[1] = v3_create(1, 0, 0);
    CELL_OFFSETS[2] = v3_create(0, 1, 0);
    CELL_OFFSETS[3] = v3_create(1, 1, 0);
    CELL_OFFSETS[4] = v3_create(0, 0, 1);
    CELL_OFFSETS[5] = v3_create(1, 0, 1);
    CELL_OFFSETS[6] = v3_create(0, 1, 1);
    CELL_OFFSETS[7] = v3_create(1, 1, 1);
}

/* Extract a cell from the volume, so that datapoint 0 of the
   cell corresponds to voxel (i, j, k), datapoint 1 to voxel (i+1, j, k),
   etc. See the assignment. */
cell
get_cell(int i, int j, int k)
{
    cell c;
    init_cell_offsets(); // wasteful, but we don't want to mess with the framework

    // cell.p contains the 8 cornerpoints of the cell, in specific order
    vec3 root = v3_create(i, j, k);
    vec3 offsets, voxel;

    for (int o = 0; o < 8; ++o) {
        offsets = CELL_OFFSETS[o];
        voxel = v3_add(root, offsets);

        c.p[o]     = voxel;
        c.n[o]     = v3_create(0, 0, 1);
        c.value[o] = volume[ voxel2idx(voxel.x, voxel.y, voxel.z) ];
    }

    return c;
}

/* Utility function to read a volume dataset from a VTK file.
   This will store the data in the "volume" array and update the dimension
   and size values. */

void
read_volume(const char *fname)
{
    FILE *f;
    char s[256];
    int nvoxels;

    printf("Reading %s\n", fname);
    f = fopen(fname, "rb");

    if (!f)
    {
        fprintf(stderr, "read_volume(): Could not open file '%s' for reading!\n", fname);
        exit(-1);
    }

    // time to surpress those fucking errors
    void *foo = NULL;
    int bar   = 0;

    if (foo && bar) {
    }

    // header line
    foo = fgets(s, 255, f);

    // comment line
    foo = fgets(s, 255, f);

    // BINARY
    foo = fgets(s, 255, f);

    // DATASET STRUCTURED_POINTS
    foo = fgets(s, 255, f);

    // DIMENSIONS %d %d %d
    bar = fscanf(f, "%s %d %d %d\n", s, &nx, &ny, &nz);
    printf("%d x %d x %d voxels\n", nx, ny, nz);

    // ASPECT_RATIO/SPACING %f %f %f
    bar = fscanf(f, "%s %f %f %f\n", s, &sizex, &sizey, &sizez);
    printf("voxel sizes: %.3f, %.3f, %.3f\n", sizex, sizey, sizez);

    // ORIGIN ...
    foo = fgets(s, 255, f);

    // POINT_DATA ...
    foo = fgets(s, 255, f);

    // SCALARS ...
    foo = fgets(s, 255, f);

    // LOOKUP_TABLE ...
    foo = fgets(s, 255, f);

    // allocate memory to hold the volume data and read it from file
    nvoxels = nx * ny * nz;
    volume = (unsigned char*)malloc(nvoxels);

    if (fread(volume, 1, nvoxels, f) < (size_t)nvoxels)
    {
        printf("WARNING: not all data could be read!\n");
    }

    fclose(f);
}
