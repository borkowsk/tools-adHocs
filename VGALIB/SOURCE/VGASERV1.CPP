/*
Ustawianie zmiennych funkcyjnych dla grafiki - BIOS
*/
#include <stdio.h>
#include "TopVga.h"
#include "Biosgraf.h"

#define RunBIOS 0
#define Run16C  1
#define Run13H  2
#define RunPRA  3
#define RunVESA 4
#define Run320x400  5
#pragma argsused
void far imageerror(long int x1, long int y1,int n,int m,unsigned char far* bufor)
{
//fprintf(stderr,"Image function not implemented !\n");
}


void  far (*setpixel)(long int x , long int y ,unsigned int color)=BiosSetPixel;
void  far (*line)(long int x1, long int y1,long int x2, long int y2)=BiosLine;
void  far (*cleardevice)(void)=BiosCleardevice;
void  far (*clearbar)(long int x1, long int y1,long int x2, long int y2)=BiosClearBar;
void  far (*fillbar)(long int x1, long int y1,long int x2, long int y2)=BiosFillBar;
void  far (*circle)(long int xxs,long int yys,long int bb)=BiosCircle;
void  far (*ellipse)(long int xxs,long int yys,long int aa,long int bb)=BiosEllipse;\
void  far (*fillellipse)(long int xxs,long int yys,long int aa,long int bb)=BiosFillEllipse;
void  far (*fillcircle)(long int xxs,long int yys,long int bb)=BiosFillCircle;
void  far (*getmap)(long int x1, long int y1,int n,int m,unsigned char far* bufor)=imageerror;
void  far (*putmap)(long int x1, long int y1,int n,int m,unsigned char far* bufor)=imageerror;

char  GRSERVICE=RunBIOS;

void  far graphicsBIOS()
{
setpixel=BiosSetPixel;
line=BiosLine;
cleardevice=BiosCleardevice;
clearbar=BiosClearBar;
fillbar=BiosFillBar;
circle=BiosCircle;
ellipse=BiosEllipse;
fillcircle=BiosFillCircle;
fillellipse=BiosFillEllipse;
putmap=imageerror;
GRSERVICE=RunBIOS;
cleardevice();
}