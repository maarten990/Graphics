/* Computer Graphics and Game Technology, Assignment Ray-tracing
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
#include <stdio.h>
#include <stdlib.h>
#include "shaders.h"
#include "perlin.h"
#include "v3math.h"
#include "intersection.h"
#include "scene.h"
#include "quat.h"
#include "constants.h"

// shade_constant()
//
// Always return the same color. This shader does no real computations
// based on normal, light position, etc. As such, it merely creates
// a "silhouette" of an object.

vec3
shade_constant(intersection_point ip)
{
    return v3_create(1, 0, 0);
}

vec3
shade_matte(intersection_point ip)
{

    // Normal vector
    vec3 n = ip.n;
    n = v3_normalize(n);
    vec3 l, p2;
    float spread, r = 0.05, g = 0.05, b = 0.05 ;
    
    // iterate trough all lights for final diffused illumination
    for(int i = 0; i < scene_num_lights; i ++)
    {

    
        l = scene_lights[i].position; 
        l = v3_normalize(l);
      //  l.x -= ip.p.x;
      //  l.y -= ip.p.y;
      //  l.z -= ip.p.z;

     //   l = v3_normalize(l);
        spread = l.x * n.x + l.y * n.y + l.z * n.z;
        if(spread < 0)
        {
           // pass 
        }
        else
        {
            r += scene_lights[i].intensity * spread;
            g += scene_lights[i].intensity * spread;
            b += scene_lights[i].intensity * spread;
        }

    }

    return v3_create(r, g, b);
}

vec3
shade_blinn_phong(intersection_point ip)
{
    return v3_create(1, 0, 0);
}

vec3
shade_reflection(intersection_point ip)
{
    return v3_create(1, 0, 0);
}

// Returns the shaded color for the given point to shade.
// Calls the relevant shading function based on the material index.
vec3
shade(intersection_point ip)
{
  switch (ip.material)
  {
    case 0:
      return shade_constant(ip);
    case 1:
      return shade_matte(ip);
    case 2:
      return shade_blinn_phong(ip);
    case 3:
      return shade_reflection(ip);
    default:
      return shade_constant(ip);

  }
}

// Determine the surface color for the first object intersected by
// the given ray, or return the scene background color when no
// intersection is found
vec3
ray_color(int level, vec3 ray_origin, vec3 ray_direction)
{
    intersection_point  ip;

    // If this ray has been reflected too many times, simply
    // return the background color.
    if (level >= 3)
        return scene_background_color;

    // Check if the ray intersects anything in the scene
    if (find_first_intersection(&ip, ray_origin, ray_direction))
    {
        // Shade the found intersection point
        ip.ray_level = level;
        return shade(ip);
    }

    // Nothing was hit, return background color
    return scene_background_color;
}
