/*
BIOS-owa obsluga karty graficznej - paleta dla wartosci funkcji
*/
#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include <DOS.h>
#include <math.h>
#include <conio.h>
#include "BIOSGRAF.h"

int far FuncContinum()
{
RGBrec pom;
int i,j,k;

if(DEVICE<7) return 0;

if(NUMCOLOR>8 && NUMCOLOR<=16)
{
unsigned char pal[18];
/* grab a copy of the palette */
BiosGetVGAPalette(pal);
/* create gray scale */
for (i=0; i<=0x0F; i++)
      BiosSetRGB(pal[i], i*4, i*4, 0);

return 1;
}
else
{
struct {unsigned char R;
	unsigned char G;
	unsigned char B;} RGBarray[257],RGB;

for(k=0;k<256;k++)
   {RGB.R=0;
    RGB.G=0;
    RGB.B=0;
    RGBarray[k]=RGB;
    }
for(k=0;k<64;k++)
    {
    RGBarray[k].R=k;
    RGBarray[k+192].R=k;
    RGBarray[k+64].G=k;
    RGBarray[k+128].B=k;
    }
for(k=0;k<64;k++)
    {
    RGBarray[k+64].R=63-k;
    RGBarray[k+128].G=63-k;
    RGBarray[k+192].B=63;
    }
BiosSetRGBBlock(0,256,(void far*)RGBarray);
return 1;
}
return 0;
}
