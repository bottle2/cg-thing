#ifndef __T2__VECTOR__H__
#define __T2__VECTOR__H__

union vector
{
    struct { int x    ; int y     ; };
    struct { int width; int height; };
};

#if 0
void normalize()
{
   float norm = (float)sqrt(x*x + y*y);

   if(norm==0.0)
   {
	  printf("\n\nNormalize::Divisao por zero");
	  x = 1;
	  y = 1;
	  return;
   }
   x /= norm;
   y /= norm;
}
#endif
// NOTE Leftover code, maybe salvage?

#endif
