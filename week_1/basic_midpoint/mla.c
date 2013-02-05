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

  int x;
  int y = y0;
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
 

  return;
}

