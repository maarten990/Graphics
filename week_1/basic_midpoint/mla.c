/* Computer Graphics, Assignment 1, Bresenham's Midpoint Line-Algorithm
 *
 * Filename ........ mla.c
 * Description ..... Midpoint Line Algorithm
 * Created by ...... Jurgen Sturm 
 *
 * Student name .... Inge Becht
 * Student email ... inge.becht91@gmail.com
 * Collegekaart .... 6093906
 * Date ............ 07-02-2013
 * Comments ........
 *
 *
 * Student name .... Maarten de Jonge
 * Student email ... mdejonge1990@gmail.com
 * Collegekaart .... 6286658
 * Date ............ 07-02-2013
 * Comments ........
 *
 * (always fill in these fields before submitting!!)
 */

#include "SDL.h"   
#include "init.h"

/*
 * Midpoint Line Algorithm
 *
 * As you probably will have figured out, this is the part where you prove
 * your programming skills. The code in the mla function should draw a direct 
 * line between (x0,y0) and (x1,y1) in the specified color. 
 * 
 * Until now, the example code below draws only a horizontal line between
 * (x0,y0) and (x1,y0) and a vertical line between (x1,y1).
 * 
 * And here the challenge begins..
 *
 * Good luck!
 *
 *
 */
void mla(SDL_Surface *s, int x0, int y0, int x1, int y1, Uint32 colour) {
    // Slope used for octant choice
    double slope;

    // Create artificial slope for the cases of -inf and inf
    if((x1 == x0) && (y1 < y0))
    {
        slope = -2;
    }
    else if((x1 == x0) && (y0 < y1))
    {
        slope = 2;
    }
    else{  

        // Standard slope calculation
        slope = (y1 - y0) / (double)(x1 - x0);
    }

    // create iteration variables
    int x;
    int y = y0;


    // octant 1 works: x0 < x1, y0 < y1, dy < dx
    if(slope >= 0 && slope <= 1 && x1 > x0)
    {
        double d =  ((y0 - y1) * (x0 + 1) + (x1 - x0) * (y0 + 0.5) + x0 * y1 - x1 * y0);



        // Loop through all the pixel in the longest direction
        for(x = x0 ; x <= x1; x++)
        {

            // Place the pixel 
            PutPixel(s,x,y,colour);
            // In  case the point is lower than the line segment at the point
            if( d < 0)
            {
                // Move y up, increment d value
                y = y + 1;
                d = d + (x1 - x0) + (y0 - y1);

            }
            else
            {
                d = d + (y0 - y1);
            }

        }
    }
    // octant 5: x1 < x0, y1 < y0, dy < dx
    else if(slope >= 0 && slope <= 1 && x1 < x0)
    {

        double d =  ((y0 - y1) * (x0 + 1) + (x1 - x0) * (y0 + 0.5) + x0 * y1 - x1 * y0);

        for(x = x0 ; x >= x1; x--)
        {

            PutPixel(s,x,y,colour);
            if( d > 0)
            {
                y = y - 1;
                d = d + (x1 - x0) + (y0 - y1);

            }
            else
            {
                d = d + (y0 - y1);
            }

        }
    }
    // Octant 2 works x0 < x1, y0 < y1, dx<dy
    else if (slope > 1 && y1 > y0){

        x = x0;
        // Distance between y0 and  y1 is bigger than x1 and x0 so swap x and y
        double d =  ((x0 - x1) * (y0 + 1) + (y1 - y0) * (x0 + 0.5) + y0 * x1 - y1 * x0);

        for(y = y0 ; y < y1; y++)
        {
            PutPixel(s,x,y,colour);
            if( d < 0)
            {
                x = x + 1;
                d = d + (y1 - y0) + (x0 - x1);

            }
            else
            {
                d = d + (x0 - x1);
            }
        }
    }
    // Octant 6 works  x1 > x0, y1 > y0 , dx < dy
    else if (slope > 1 && y1 <= y0  )
    {
        printf("slope = %f\n", slope);
        x = x0;
        double d =  ((x0 - x1) * (y0 + 1) + (y1 - y0) * (x0 + 0.5) + y0 * x1 - y1 * x0);

        for(y = y0 ; y >= y1; y--)
        {
            PutPixel(s,x,y,colour);
            if( d > 0)
            {
                x = x - 1;
                d = d + (y1 - y0) + (x0 - x1);

            }
            else
            {
                d = d + (x0 - x1);
            }
        }
    }

    //octant 3 works x1 < x0, y0 < y1, dx < dy
    else if (slope < - 1 && x1 < x0)
    {

        x = x0;
        // x1 is lower than x0 and dy > dx so again we iterate over y
        double d =  ((x0 - x1) * (y0 + 1) + (y1 - y0) * (x0 - 0.5) + y0 * x1 - y1 * x0);

        for(y = y0 ; y <= y1; y++)
        {
            PutPixel(s,x,y,colour);
            if( d > 0)
            {
                x = x - 1;
                // Change sequence because of x1 < x0
                d = d + (y0 - y1) + (x0 - x1);

            }
            else
            {
                d = d + (x0 - x1);
            }
        }
    }

    //octant 7 works almost x0 < x1, y1 < y0 ,dx < dy
    else if (slope <= - 1 && x1 >= x0) 
    {
        printf("slope = %f", slope);
        x = x0;

        // iterate over y because of dx < dy
        double d =  ((x0 - x1) * (y0 + 1) + (y1 - y0) * (x0 - 0.5) + y0 * x1 - y1 * x0);

        for(y = y0 ; y >= y1; y--)
        {
            PutPixel(s,x,y,colour);
            if( d < 0)
            {
                x = x + 1;
                d = d + (y0 - y1) + (x0 - x1);

            }
            else
            {
                d = d + (x0 - x1);
            }
        }
    }

    // octant 8 Works x0 < x1, y1 < y0 dy < dx
    else if(slope >= -1 && slope <= 0 && x1 > x0)
    {

        double d =  ((y0 - y1) * (x0 + 1) + (x1 - x0) * (y0 - 0.5) + x0 * y1 - x1 * y0);

        for(x = x0 ; x <= x1; x++)
        {

            PutPixel(s,x,y,colour);
            if( d > 0)
            {
                y = y - 1;
                d = d + (x0 - x1) + (y0 - y1);

            }
            else
            {
                d = d + (y0 - y1);
            }

        }
    }
    // Octant 4 works: x1 < x0, y0 < y1 dy < dx
    else if(slope >= -1 && slope <= 0 && x1 < x0)
    {

        double d =  ((y0 - y1) * (x0 + 1) + (x1 - x0) * (y0 - 0.5) + x0 * y1 - x1 * y0);

        for(x = x0 ; x >= x1; x--)
        {

            PutPixel(s,x,y,colour);
            if( d < 0)
            {
                y = y + 1;
                d = d + (x0 - x1) + (y0 - y1);

            }
            else
            {
                d = d + (y0 - y1);
            }
        }

    }

    return;
}

