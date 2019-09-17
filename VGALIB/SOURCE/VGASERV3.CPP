/*
Ustawianie zmiennych funkcyjnych dla grafiki - tryb 13H
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


void  far graphics13H()
{
setpixel=SetPixel13H;
line=Line13H;
cleardevice=Cleardevice13H;
clearbar=ClearBar13H;
fillbar=FillBar13H;
circle=Circle13H;
ellipse=Ellipse13H;
fillcircle=FillCircle13H;
fillellipse=FillEllipse13H;
putmap=PutMap13H;
GRSERVICE=Run13H;
cleardevice();
}
