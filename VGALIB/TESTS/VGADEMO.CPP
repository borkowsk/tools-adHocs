#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <mem.h>
#include <stdlib.h>
#include <string.h>
#include "BIOSGRAF.h"
#include "TopVGA.h"
#include "vesa.hpp"

void Demo();
void DemoEGA();

main()
{
randomize();
BiosDevices();
if(BiosScreen(13))
	{
	graphics16C();
	Demo();
	BiosScreen(0x3); /* VGA 25/80 text mode */
	}
	else
	{
	printf("EGA requaried\n");
	return 1;
	}
if(BiosScreen(0x13))
	{
	graphics13H();
	Demo();
	BiosScreen(0x3); /* VGA 25/80 text mode */
	}
	else
	{
	printf("VGA requaried\n");
	return 1;
	}
if(BiosScreen(0x100))
	{
	graphicsVESA();
	Demo();
	BiosScreen(0x3); /* VGA 25/80 text mode */
	}
	else
    if(BiosScreen(0x101))
	{
	graphicsVESA();
	Demo();
	BiosScreen(0x3); /* VGA 25/80 text mode */
	}
	else
	{
	printf("VESA SVGA requaried\n");
	return 1;
	}
}

void Demo()
{
int  x=MAXX/2,y=MAXY/2;
signed char r=MAXY/10+1,xrel=random(r)+1,yrel=random(r)+1;
unsigned char color=0;
for(int i=256/2;i>0;i--)
	{
	COLOR=color+=2;
	fillcircle(MAXX/2,MAXY/2,i);
	}

ColorContinum(GB_R);
delay(500);
ColorContinum(BR_G);
delay(500);
ColorContinum(RG_B);
delay(1000);
FuncContinum();
delay(1000);
do{
COLOR=color++;
r++;
r%=20;
fillcircle(x,y,r);
x+=xrel;y+=yrel;
if(x<0 || x>MAXX){xrel=-xrel;}
if(y<0 || y>MAXY){yrel=-yrel;}
if(random(100)==0){
		xrel=xrel +(random(5)-random(5));
		yrel=yrel +(random(5)-random(5));
		}
}while(!kbhit());
getch();
}

void DemoEGA()
{
int x=MAXX/2,y=MAXY/2,r=20/*random(40)+1*/,xrel=random(r)+1,yrel=random(r)+1;
unsigned char color=random(16);
do{
COLOR=color++;
r++;
r%=20;
fillcircle(x,y,r);
x+=xrel;y+=yrel;
if(x<0 || x>MAXX){xrel=-xrel;}
if(y<0 || y>MAXY){yrel=-yrel;}
if(random(100)==0){
		xrel=xrel +(random(5)-random(5));
		yrel=yrel +(random(5)-random(5));
		}
}while(!kbhit());
getch();
}

