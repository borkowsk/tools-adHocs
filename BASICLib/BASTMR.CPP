#include <dos.h>
#include <math.h>
#include <conio.h>

float	 timer()
{
   struct time t;
   gettime (&t);
   return ((float) t.ti_hour*3600 + (float) t.ti_min*60 + 
	   (float) t.ti_sec + (float) t.ti_hund/100)
;
}

