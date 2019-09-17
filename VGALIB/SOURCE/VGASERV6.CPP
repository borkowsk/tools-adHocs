/*
Ustawianie zmiennych funkcyjnych dla grafiki - VESA 256 color
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

void far imageerror(long int x1, long int y1,int n,int m,unsigned char far* bufor);

extern void  far (*setpixel)(long int x , long int y ,unsigned int color);
extern void  far (*line)(long int x1, long int y1,long int x2, long int y2);
extern void  far (*cleardevice)(void);
extern void  far (*clearbar)(long int x1, long int y1,long int x2, long int y2);
extern void  far (*fillbar)(long int x1, long int y1,long int x2, long int y2);
extern void  far (*circle)(long int xxs,long int yys,long int bb);
extern void  far (*ellipse)(long int xxs,long int yys,long int aa,long int bb);
extern void  far (*fillellipse)(long int xxs,long int yys,long int aa,long int bb);
extern void  far (*fillcircle)(long int xxs,long int yys,long int bb);
extern void  far (*getmap)(long int x1, long int y1,int n,int m,unsigned char far* bufor);
extern void  far (*putmap)(long int x1, long int y1,int n,int m,unsigned char far* bufor);

extern char  GRSERVICE;

void  far graphicsVESA()
{
setpixel=SetPixelVESA;
line=LineVESA;
cleardevice=CleardeviceVESA;
clearbar=ClearBarVESA;
fillbar=FillBarVESA;
circle=CircleVESA;
ellipse=EllipseVESA;
fillcircle=FillCircleVESA;
fillellipse=FillEllipseVESA;
putmap=PutMapVESA;
GRSERVICE=RunVESA;
cleardevice();
}
