/*
Ustawianie zmiennych funkcyjnych dla grafiki - 320x400x256 color
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

void  far graphics320x400()
{
setpixel=SetPixel320x400;
line=Line320x400;
cleardevice=Cleardevice320x400;
clearbar=ClearBar320x400;
fillbar=FillBar320x400;
circle=Circle320x400;
ellipse=Ellipse320x400;
fillcircle=FillCircle320x400;
fillellipse=FillEllipse320x400;
putmap=PutMap320x400;
GRSERVICE=Run320x400;
cleardevice();
}
