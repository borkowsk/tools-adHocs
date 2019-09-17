/*
test biblioteki WUI - procedury obslugi ekranu
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <values.h>
#include <math.h>
#include <conio.h>

#include "biosgraf.h"
#include "picture.hpp"
#include "screens.hpp"
#include "fonts.hpp"

#define BREAK if(kbhit()){getch();break;}

void error(char* p,int c);


extern mem_picture B,I;

void ColorTest(picture& S)
{
int i,j;
int krokX,krokY;
krokX=S.GetMaxX()/16;
krokY=(S.GetMaxY())/17;
if(krokX==0) krokX=1;
if(krokY==0) krokY=1;
for(i=0;i<256;i++)
	{
	S.SetColor(i);
	S.DrawLine(i,S.GetMaxY(),i,S.GetMaxY()-krokY);
	}
for(i=0;i<16;i++)
    for(j=0;j<16;j++)
	{
	RGB rgb(0,i*16,j*16);
	color_handle c=S.GetPalette().Alloc(rgb,4);
	S.SetColor(c);
	S.FillRectAngle(i*krokX,j*krokY,(i+1)*krokX,(j+1)*krokY);
	BREAK;
	}
delay(500);

for(i=0;i<16;i++)
    for(j=0;j<16;j++)
	{
	RGB rgb(i*16,0,j*16);
	color_handle c=S.GetPalette().Alloc(rgb,4);
	S.SetColor(c);
	S.FillRectAngle(i*krokX,j*krokY,(i+1)*krokX,(j+1)*krokY);
	BREAK;
	}
delay(500);
for(i=0;i<16;i++)
    for(j=0;j<16;j++)
	{
	RGB rgb(i*16,j*16,0);
	color_handle c=S.GetPalette().Alloc(rgb,4);
	S.SetColor(c);
	S.FillRectAngle(i*krokX,j*krokY,(i+1)*krokX,(j+1)*krokY);
	BREAK;
	}
delay(500);
/*
RGB white(255,255,255);
color_handle white_handle=S.GetPalette().Alloc(white,4);
for(i=0;i<16;i++)
	{
	RGB gray(i*16,i*16,i*16);
        color_handle gray_handle=S.GetPalette().Alloc(gray,4);
	Window W(S,S.GetMaxX()/4,S.GetMaxY()/4,S.GetMaxX()/2,S.GetMaxY()/2);
	W.SetColor(white_handle);
        W.SetBackground(
	BREAK;
	}
*/
}
