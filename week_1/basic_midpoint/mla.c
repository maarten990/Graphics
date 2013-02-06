/* Computer Graphics, Assignment 1, Bresenham's Midpoint Line-Algorithm
 *
 * Filename ........ mla.c
 * Description ..... Midpoint Line Algorithm
 * Created by ...... Jurgen Sturm 
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
    // Slope
    double slope;

    if(x1 == x0 && x1> x0)
    {
        slope = 1000;
    }
    else{  
    slope = (y1 - y0) / (double)(x1 - x0);
    }
    // Put pixels at the end and beginning
    printf("The slope: %f\n", slope);
    PutPixel(s,x0,y0,colour);
    PutPixel(s,x1,y1,colour);

    int x;
    int y = y0;


    // octant 1 works: x0 < x1, y0 < y1
    if(slope >= 0 && slope < 1 && x1 > x0)
    {
        double d =  ((y0 - y1) * (x0 + 1) + (x1 - x0) * (y0 + 0.5) + x0 * y1 - x1 * y0);



        for(x = x0 ; x < x1; x++)
        {

            PutPixel(s,x,y,colour);
            if( d < 0)
            {
                y = y + 1;
                d = d + (x1 - x0) + (y0 - y1);

            }
            else
            {
                d = d + (y0 - y1);
            }

        }
        PutPixel(s,x1,y1, colour);
    }
    // octant 5: x1 < x0, y1 < y0
    else if(slope >= 0 && slope <1 && x1 < x0)
    {

        double d =  ((y0 - y1) * (x0 + 1) + (x1 - x0) * (y0 + 0.5) + x0 * y1 - x1 * y0);

        for(x = x0 ; x > x1; x--)
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
        PutPixel(s,x1,y1, colour);




    }



    // Octant 2 works x0 < x1, y0 < y1
    else if (slope >= 1 && y1 > y0){


        x = x0;

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
    // Octant 6 works  x1 > x0, y1 > y0
    else if (slope >= 1 && y1 < y0)
    {


        x = x0;

        double d =  ((x0 - x1) * (y0 + 1) + (y1 - y0) * (x0 + 0.5) + y0 * x1 - y1 * x0);

        for(y = y0 ; y > y1; y--)
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


   
    //octant 3 works
    else if (slope < - 1 && x1 < x0)
    {
      
        x = x0;

        printf("slope %f \n", slope);
        double d =  ((x0 - x1) * (y0 + 1) + (y1 - y0) * (x0 - 0.5) + y0 * x1 - y1 * x0);

        for(y = y0 ; y < y1; y++)
        {
            PutPixel(s,x,y,colour);
            if( d > 0)
            {
                x = x - 1;
                d = d + (y0 - y1) + (x0 - x1);

            }
            else
            {
                d = d + (x0 - x1);
            }

        }

    }
    //octant 7 works almost
    else if (slope <= - 1 && x1 > x0) 
    {
        printf("slope = %f", slope);
             x = x0;

        double d =  ((x0 - x1) * (y0 + 1) + (y1 - y0) * (x0 + 0.5) + y0 * x1 - y1 * x0);

        for(y = y0 ; y > y1; y--)
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


    // octant 8 Works x0 < x1, y1 < y0
    else if(slope >= -1 && slope < 0 && x1 > x0)
    {

    double d =  ((y0 - y1) * (x0 + 1) + (x1 - x0) * (y0 - 0.5) + x0 * y1 - x1 * y0);

        for(x = x0 ; x < x1; x++)
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
        PutPixel(s,x1,y1, colour);
    }
    // Octant 4 works: x1 < x0, y0 < y1
    else if(slope >= -1 && slope < 0 && x1 < x0)
    {

    double d =  ((y0 - y1) * (x0 + 1) + (x1 - x0) * (y0 - 0.5) + x0 * y1 - x1 * y0);

        for(x = x0 ; x > x1; x--)
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
        PutPixel(s,x1,y1, colour);

    }

    return;
}

