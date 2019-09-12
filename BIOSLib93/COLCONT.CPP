/*
BIOS-owa obsluga karty graficznej
- ciagla paleta kolorow
*/
#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include <DOS.h>
#include <math.h>
#include <conio.h>
#include "BIOSGRAF.h"

int far ColorContinum(int typ)
{
RGBrec pom;
unsigned int i,j;
struct {unsigned char R;
	unsigned char G;
	unsigned char B;} RGBarray[257],RGB;

if(NUMCOLOR<256) return 0;

switch( typ )
{
case RG_B :
for(i=0;i<256;i++)
  {pom=*(RGBrec *)&i;
   RGB.R=(pom.I*MUL_I);RGB.G=(pom.II*MUL_II);RGB.B=(pom.III*MUL_III);
   RGBarray[i]=RGB;}
break;
case BR_G :
for(i=0;i<256;i++)
  {pom=*(RGBrec *)&i;
   RGB.B=(pom.I*MUL_I);RGB.R=(pom.II*MUL_II);RGB.G=(pom.III*MUL_III);
   RGBarray[i]=RGB;}
break;
case GB_R :
for(i=0;i<256;i++)
  {pom=*(RGBrec *)&i;
   RGB.G=(pom.I*MUL_I);RGB.B=(pom.II*MUL_II);RGB.R=(pom.III*MUL_III);
   RGBarray[i]=RGB;}
break;
default:
return 0;
}
BiosSetRGBBlock(0,256,RGBarray);
return 1;
}
