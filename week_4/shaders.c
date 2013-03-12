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

    // check the shadow ray, starting slightly above the surface to avoid
    // self-shading
    if (shadow_check(v3_add(ip.p, v3_multiply(ip.n, 0.01)), l))
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

    // check the shadow ray, starting slightly above the surface to avoid
    // self-shading
    if (shadow_check(v3_add(ip.p, v3_multiply(ip.n, 0.01)), l))
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
<<<<<<< HEAD
    float color = scene_ambient_light;

    // loop through each lightsource
    for (int i = 0; i < scene_num_lights; ++i) {
        color += diffuse_term(ip, scene_lights[i]);
    }

    return v3_create(color, color, color);
=======

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
>>>>>>> d4dca791ddbd04535e0cfbe4cb377ba0120b3a79
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
    // specular channel affects red, greed ed en and blue equally (thus it is
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


// Create matt shading with reflections
vec3
shade_reflection(intersection_point ip)
{
<<<<<<< HEAD
    float angle = 0, colour = 0, original, reflected;

    // Reflect direction
    vec3 r, temp;

    // Add lighting effect of all light sources
    for(int i = 0; i < scene_num_lights; i ++ )
    {

        angle = v3_dotprod(ip.i, ip.n);
        temp = v3_multiply(v3_multiply(ip.n, angle), 2);
        r =  v3_subtract(temp, ip.i);
        r = v3_normalize(r);

        original = 0.75 * diffuse_term(ip, scene_lights[i]);
        if(original < 0 )
        {
            printf("\n\n\n\n\n IS SMALLER THAN ZERO\n\n\n");
            
        }
        reflected =  ray_color(ip.ray_level + 1, ip.p, r).x * 0.25;
        if ( reflected == (scene_background_color.x * 0.25 ))
        {
            reflected = 0;
            
        }
        colour += (original + reflected);
        
    }
    return v3_create(colour, colour, colour);
=======
    // calculate the reflection ray
    vec3 r = v3_multiply(ip.n, 2);
    r = v3_multiply(r, v3_dotprod(ip.i, ip.n));
    r = v3_subtract(r, ip.i);
    r = v3_normalize(r);

    // the reflection color is found by further tracing the reflection ray from
    // the current location
    vec3 reflection_color = ray_color(ip.ray_level + 1, v3_add(ip.p, v3_multiply(ip.n, 0.01)), r),
         final_color;

    float matte_color = 0;
    for (int i = 0; i < scene_num_lights; ++i) {
        matte_color += diffuse_term(ip, scene_lights[i]);
    }

    // the color used it 75% of the matte shading, and 25% of the reflected
    // color
    final_color = v3_create(0.25 * reflection_color.x + 0.75 * matte_color,
                            0.25 * reflection_color.y + 0.75 * matte_color,
                            0.25 * reflection_color.z + 0.75 * matte_color);

    return final_color;
>>>>>>> 98910e154fd1f2c0cb1fb40e253b7139c0f800ce
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
