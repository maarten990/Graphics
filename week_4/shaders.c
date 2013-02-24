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

/**
 * Returns the lambertian shading of an intersection point with a single
 * lightsource, according to the formula:
 *
 * L = kd * I * max(0, n dot l)
 * where L is the resulting color, kd is the diffuse coefficient (surface
 * color), I is the light intensity, n is the surface normal vector, and l
 * points towards the light source (l = location of lightsource -
 * intersection point). Make sure every vector involved is normalised.
 */
float
diffuse_term(intersection_point ip, light lsource)
{
    vec3 l;
    float angle;
    
    l = v3_normalize( v3_subtract(lsource.position, ip.p) );

    // check the shadow ray
    if (shadow_check(v3_multiply(ip.p, 1.001), l))
        return 0;

    // if the dotproduct is negative, the lightsource is behind the surface
    angle = v3_dotprod(ip.n, l);
    angle = angle > 0 ? angle : 0;

    return lsource.intensity * angle;
}

/**
 * Returns the Blinn-Phong shading term of an intersection points with a single
 * lightsource, according to the formula:
 *
 * L = I * max(0, n dot h)^alpha
 * where:
 * v = viewpoint
 * l = lightsource
 * h = halfway-vector between v and l
 * alpha = Blinn-Phong constant
 */
float
specular_term(intersection_point ip, light lsource, float alpha)
{
    vec3 l, h, v_plus_l;
    float angle;
    
    l = v3_normalize( v3_subtract(lsource.position, ip.p) );

    // check the shadow ray
    if (shadow_check(v3_multiply(ip.p, 1.001), l))
        return 0;

    // calculate h
    v_plus_l = v3_add(ip.i, l);
    h = v3_multiply(v_plus_l, 1 / v3_length(v_plus_l));

    // if the dotproduct is negative, the lightsource is behind the surface
    angle = v3_dotprod(ip.n, h);
    angle = angle > 0 ? angle : 0;

    return lsource.intensity * pow(angle, alpha);
}

vec3
shade_matte(intersection_point ip)
{
    float color = scene_ambient_light;

    // loop through each lightsource
    for (int i = 0; i < scene_num_lights; ++i) {
        color += diffuse_term(ip, scene_lights[i]);
    }

    return v3_create(color, color, color);
}

vec3
shade_blinn_phong(intersection_point ip)
{
    /**
     * Blinn-Phong shading:
     * L = kd * I * max(0, n dot l) + ks * I * max(0, n dot h)^p
     */

    // some parameter values
    float kd    = 0.8,
          ks    = 0.5,
          alpha = 50;
    
    // the diffuse shading only affects the red color channel, while the
    // specular channel affects red, green and blue equally (thus it is
    // white-coloured)
    float all = 0,
          red = scene_ambient_light;

    for (int i = 0; i < scene_num_lights; ++i) {
        red += diffuse_term(ip, scene_lights[i]);
        all += specular_term(ip, scene_lights[i], alpha);
    }

    // weighing factor for the two terms
    red *= kd;
    all *= ks;

    // make sure the colors stay within the [0, 1] interval
    red += all;
    if (red > 1)
        red = 1;

    return v3_create(red, all, all);
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
