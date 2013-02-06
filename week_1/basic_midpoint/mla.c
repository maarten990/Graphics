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


    printf("here\n");   

    // Slope
    double slope;
    // The additional y value for the line
    double addition;

    slope = (y1 - y0) / (double)(x1 - x0);
    addition = y0 - (slope * x0);
    // Put pixels at the end and beginning
    printf("The slope: %f", slope);
    printf("The addition: %f", addition);
    PutPixel(s,x0,y0,colour);
    PutPixel(s,x1,y1,colour);

    int x;
    int y = y0;


    double d =  ((y0 - y1) * (x0 + 1) + (x1 - x0) * (y0 + 0.5) + x0 * y1 - x1 * y0);

    if(slope >= 0 && slope < 1)
    {
        printf("slope is between 1 and 0 \n\n");

        for(x = x0 ; x < x1; x++)
        {

            printf("d: %f\n", d);
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



    // In case slope is between 1 and infinity
    else if (slope >= 1){

        printf("here\n");
        printf("x1-x0: %d\n", x1-x0);
        printf("y0-y1: %d\n", y0-y1);
        printf("y1-y0: %d\n", y1-y0);
        printf("x0-x1: %d\n", x0-x1);



        x = x0;

        double d =  ((x0 - x1) * (y0 + 1) + (y1 - y0) * (x0 + 0.5) + y0 * x1 - y1 * x0);

        for(y = y0 ; y < y1; y++)
        {
            printf("d: %f\n", d);
            PutPixel(s,x,y,colour);
            if( d < 0)
            {
                printf("do y");
                x = x + 1;
                d = d + (y1 - y0) + (x0 - x1);

            }
            else
            {
                d = d + (x0 - x1);
            }

        }

    }


    else if(slope >= -1 && slope < 0)
    {

    double d =  ((y0 - y1) * (x0 + 1) + (x1 - x0) * (y0 - 0.5) + x0 * y1 - x1 * y0);
      printf("slope is between 1 and 0 \n\n");

        for(x = x0 ; x < x1; x++)
        {

            printf("d: %f\n", d);
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


    return;
}

