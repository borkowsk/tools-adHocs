/*
Bibliotek klasycznego interfacu do grafiki
Ustawianie zmiennych funkcyjnych dla grafiki - reczne ustawianie
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
void graphics16C();
void graphics13H();
void graphicsPRA();
void graphicsVESA();
void graphics320x400();
void graphicsBIOS();

void  far ManualSetService()
{
printf("Graphics services:\n 1 - color 16\n 2 - mode 13H\n"
			   " 3 - via PROA reg.\n 4 - VESA with 256 colors\n"
			   " 5 - 320x400 256 col.\n"
       " else BIOS \n");
switch(getchar())
{
case '1':graphics16C();break;
case '2':graphics13H();break;
case '3':graphicsPRA();break;
case '4':graphicsVESA();break;
case '5':graphics320x400();break;
default:graphicsBIOS();
}
(getchar)();
}

