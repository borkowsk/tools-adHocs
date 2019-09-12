/*
BIOS-owa obsluga karty graficznej -
podstawowe funkcje palety
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
