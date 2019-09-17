/*
BIOS-owa obsluga karty graficznej - paleta kolorow VGA
*/
#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include <DOS.h>
#include <math.h>
#include <conio.h>
#include "BIOSGRAF.h"

void far BiosGetVGAPalette(unsigned char far *pal)
{
_ES=FP_SEG(pal);
_DX=FP_OFF(pal);
_AH=0x10;
_AL=9;
geninterrupt(0x10);
}

void far BiosSetRGB(unsigned int n,unsigned  char R,unsigned  char G,unsigned  char B)
{
_BX=n;
_CH=G;
_CL=B;
_DH=R;
_AH=0x10;
_AL=0x10;
geninterrupt(0x10);
}

void far BiosSetRGBBlock(unsigned int s,unsigned int n,void far* table)
{
_BX=s; /* startujac od rejestru s */
_CX=n; /* ustawiamy n rejestrow */
_ES=FP_SEG(table);
_DX=FP_OFF(table);
_AH=0x10;
_AL=0x12;
geninterrupt(0x10);
}

void far BiosGetRGBBlock(unsigned int s,unsigned int n,void far* table)
{
_BX=s; /* startujac od rejestru s */
_CX=n; /* pobieramy n rejestrow */
_ES=FP_SEG(table);
_DX=FP_OFF(table);
_AH=0x10;
_AL=0x17;
geninterrupt(0x10);
}

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

int far FuncContinum()
{
RGBrec pom;
int i,j,k;

if(DEVICE<7)
	return 0;

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
// return 0;
}
