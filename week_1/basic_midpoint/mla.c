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

int bigger(double d, int x)
{
    return d > x;
}


int smaller(double d, int x)
{
    return d < x;
}

void mla(SDL_Surface *s, int x0, int y0, int x1, int y1, Uint32 colour) {
    // Slope
    double slope;

    if(x1 == x0 && y1< y0)
    {
        slope = 2;
    }
    else{  
        slope = (y1 - y0) / (double)(x1 - x0);
    }
    // Put pixels at the end and beginning
    //printf("The slope: %f\n", slope);

    int x;
    int y = y0;
    double dx = abs(x1 - x0);
    double dy = abs(y1 - y0);

 
    // octant 1 works: x0 < x1, y0 < y1, dy < dx
    if(slope >= 0 && slope <= 1 && x1 > x0)
    {
        printf( "octant 1\n" );
        double d =  ((y0 - y1) * (x0 + 1) + (x1 - x0) * (y0 + 0.5) + x0 * y1 - x1 * y0);
int  increment = 1;
if(x1 < x0)
{
    double temp = x1;
    x1 = x0;
    x0 = temp;
    y = y1;
    if(y1 > y0)
    {
        increment = -1;
    } 

}
else if(y1 < y0)
    {
        increment = -1;
    } 

        for(x = x0 ; x < x1; x++)
        {


            PutPixel(s,x,y,colour);
            if( d < 0)
            {
                y = y + increment;
                d = d + (x1 - x0) + (y0 - y1);

            }
            else
            {
                d = d + (y0 - y1);
            }

        }
        PutPixel(s,x1,y1, colour);
    }

    // octant 5: x1 < x0, y1 < y0, dy < dx
    else if(slope >= 0 && slope <= 1 && x1 < x0)
    {
        printf( "octant 5\n" );


int  increment = 1;
if(x1 < x0)
{
    double temp = x1;
    x1 = x0;
    x0 = temp;
    y = y1;
    if(y1 > y0)
    {
        increment = -1;
    } 

}
else if(y1 < y0)
    {
        increment = -1;
    } 



        double d =  ((y0 - y1) * (x0 + 1) + (x1 - x0) * (y0 - 0.5) + x0 * y1 - x1 * y0);

        for(x = x0 ; x < x1; x++)
        {

            PutPixel(s,x,y,colour);
            if( d > 0)
            {
                y = y + increment;
                d = d + (x0 - x1) + (y0 - y1);

            }
            else
            {
                d = d + (y0 - y1);
            }

        }
        PutPixel(s,x1,y1, colour);

    }


    // Octant 2 works x0 < x1, y0 < y1, dx<dy
    else if (slope >= 1 && y1 > y0){

        printf( "octant 2\n" );

        double increment = 1;
    if(dx < dy || dx == 0)
        {
            x = y0;
            y = x0;
            double temp = x0;
            x0 = y0;
            y0 = temp;
            temp = x1;
            x1 = y1;
            y1 = temp;
        }
if(x1 < x0)
{
    double temp = x1;
    x1 = x0;
    x0 = temp;
    y = y1;
    if(y1 > y0)
    {
        increment = -1;
    } 

}
else if(y1 < y0)
    {
        increment = -1;
    } 

       
        double d =  ((y0 - y1) * (x0 + 1) + (x1 - x0) * (y0 + 0.5) + x0 * y1 - x1 * y0);



        for(x = x0 ; x < x1; x++)
        {

            PutPixel(s,y,x,colour);
            if( d < 0)
            {
                y = y + increment;
                d = d + (x1 - x0) + (y0 - y1);

            }
            else
            {
                d = d + (y0 - y1);
            }

        }
        PutPixel(s,y1,x1, colour);


    }
    // Octant 6 works  x1 > x0, y1 > y0 , dx < dy
    else if (slope > 1 && y1 < y0)
    { 
        

double increment =1;

  
    if(dx < dy || dx == 0)
        {
            x = y0;
            y = x0;
            double temp = x0;
            x0 = y0;
            y0 = temp;
            temp = x1;
            x1 = y1;
            y1 = temp;
        }

if(x1 < x0)
{
    double temp = x1;
    x1 = x0;
    x0 = temp;
    y = y1;
    if(y1 > y0)
    {
        increment = -1;
    } 

}
else if(y1 < y0)
    {
        increment = -1;
    } 

        printf( "octant 6\n" );
    
        double d =  ((y0 - y1) * (x0 + 1) + (x1 - x0) * (y0 - 0.5) + x0 * y1 - x1 * y0);

        for(x = x0 ; x < x1; x++)
        {

            PutPixel(s,y,x,colour);
            if( d > 0)
            {
                y = y + increment;
                d = d + (x0 - x1) + (y0 - y1);

            }
            else
            {
                d = d + (y0 - y1);
            }

        }
        PutPixel(s,y1,x1, colour);


    }



    //octant 3 works x1 < x0, y0 < y1, dx < dy
    else if (slope < - 1 && x1 < x0)
    { 
        
    double increment = 1;
    if(dx < dy || dx == 0)
        {
            x = y0;
            y = x0;
            double temp = x0;
            x0 = y0;
            y0 = temp;
            temp = x1;
            x1 = y1;
            y1 = temp;
        }

if(x1 < x0)
{
    double temp = x1;
    x1 = x0;
    x0 = temp;
    y = y1;
    if(y1 > y0)
    {
        increment = -1;
    } 

}
else if(y1 < y0)
    {
        increment = -1;
    } 
        printf( "octant 3\n" );


        double d =  ((y0 - y1) * (x0 + 1) + (x1 - x0) * (y0 - 0.5) + x0 * y1 - x1 * y0);

        for(x = x0 ; x < x1; x++)
        {

            PutPixel(s,y,x,colour);
            if( d > 0)
            {
                y = y + increment;
                d = d + (x0 - x1) + (y0 - y1);

            }
            else
            {
                d = d + (y0 - y1);
            }

        }
        PutPixel(s,y0,x1, colour);

    }
    //octant 7 works almost x0 < x1, y1 < y0 ,dx < dy
    else if (slope < - 1 && x1 > x0) 
    {
        printf( "octant 7\n" );


double increment = 1;

    
 if(dx < dy || dx == 0)
        {
            x = y0;
            y = x0;
            double temp = x0;
            x0 = y0;
            y0 = temp;
            temp = x1;
            x1 = y1;
            y1 = temp;
        }
if(x1 < x0)
{
    double temp = x1;
    x1 = x0;
    x0 = temp;
    y = y1;
    if(y1 > y0)
    {
        increment = -1;
    } 

}
else if(y1 < y0)
    {
        increment = -1;
    } 

        double d =  ((y0 - y1) * (x0 + 1) + (x1 - x0) * (y0 - 0.5) + x0 * y1 - x1 * y0);

        for(x = x0 ; x < x1; x++)
        {

            PutPixel(s,y,x,colour);
            if( d < 0)
            {
                y = y + increment;
                d = d + (x1 - x0) + (y0 - y1);

            }
            else
            {
                d = d + (y0 - y1);
            }

        }
        PutPixel(s,y0,x1, colour);

    }


    // octant 8 Works x0 < x1, y1 < y0 dy < dx
    else if(slope >= -1 && slope <= 0 && x1 > x0)
    {
        double increment = 1;
 if(dx < dy || dx == 0)
        {
            x = y0;
            y = x0;
            double temp = x0;
            x0 = y0;
            y0 = temp;
            temp = x1;
            x1 = y1;
            y1 = temp;
        }
if(x1 < x0)
{
    double temp = x1;
    x1 = x0;
    x0 = temp;
    y = y1;
    if(y1 > y0)
    {
        increment = -1;
    } 

}
else if(y1 < y0)
    {
        increment = -1;
    } 


        printf( "octant 8\n" );

        double d =  ((y0 - y1) * (x0 + 1) + (x1 - x0) * (y0 - 0.5) + x0 * y1 - x1 * y0);

        for(x = x0 ; x < x1; x++)
        {

            PutPixel(s,x,y,colour);
            if( d > 0)
            {
                y = y + increment;
                d = d + (x0 - x1) + (y0 - y1);

            }
            else
            {
                d = d + (y0 - y1);
            }

        }
        PutPixel(s,x1,y1, colour);
    }
    // Octant 4 works: x1 < x0, y0 < y1 dy < dx
    else if(slope >= -1 && slope < 0 && x1 < x0)
    {
        //int (*compare)(int, int) = & bigger;
        double increment = 1;
 if(dx < dy || dx == 0)
        {
            x = y0;
            y = x0;
            double temp = x0;
            x0 = y0;
            y0 = temp;
            temp = x1;
            x1 = y1;
            y1 = temp;
        }

if(x1 < x0)
{
    double temp = x1;
    x1 = x0;
    x0 = temp;
    y = y1;
    if(y1 > y0)
    {
        increment = -1;
    } 

}


        printf( "octant 4\n" );

        double d =  ((y0 - y1) * (x0 + 1) + (x1 - x0) * (y0 + 0.5) + x0 * y1 - x1 * y0);

        for(x = x0 ; x < x1; x++)
        {

            PutPixel(s,x,y,colour);
            if( d < 0)
            {
                y = y + increment;
                d = d + (x1 - x0) + (y0 - y1);

            }
            else
            {
                d = d + (y0 - y1);
            }

        }
        PutPixel(s,x0,y0, colour);

    }

    return;
}


