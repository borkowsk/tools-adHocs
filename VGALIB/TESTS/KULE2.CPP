
#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <mem.h>
#include <stdlib.h>
#include <string.h>
#include "BIOSGRAF.h"
#include "BIOSCHAR.hpp"
#include "VESA.HPP"
#include "TopVGA.h"
#include "color.hpp"

void filling(int x,int y)
{
static color c;
int i,modik=random(7)+1,oldcolor=0;
int A=random(64),B=random(64);
for(i=63;i>0;i-=2)
	{
	switch(modik){
	case 1:COLOR=c.allocnear(A,B,i);break;
	case 2:COLOR=c.allocnear(A,i,B);break;
	case 3:COLOR=c.allocnear(i,A,B);break;
	case 4:COLOR=c.allocnear(A,i,i);break;
	case 5:COLOR=c.allocnear(i,A,i);break;
	case 6:COLOR=c.allocnear(i,i,A);break;
	case 7:COLOR=c.allocnear(i,i,i);break;
	}
	if(oldcolor!=COLOR)
		fillellipse(x,y,i,63);
	oldcolor=COLOR;
	}
}
main()
{
int mode,CUBESIZE=1;
#define COLORJMP (64/CUBESIZE)
int i,j,k,n=6,skokx,skoky;
color c;
char *end,tab[30];
randomize();
printf("Podaj nr trybu graficznego:");
gets(tab);
mode=strtol( tab, &end , 0 );
printf("Podaj  rozmiar \"boku\" kostki kolorow:");
gets(tab);
CUBESIZE=strtol( tab, &end , 0 );
ManualSetService();
BiosScreen(mode);
cleardevice();
color::fromhardware();
color::direct(1);
color::cube(CUBESIZE);
for(i=0;i<256;i++)
	{
	COLOR=i;
	line(MAXX-20,i,MAXX,i);
	}
MAXX-=20;
filling(MAXX/2,MAXY/2);
while(!kbhit())
	{
	i=random(MAXX);
	j=random(MAXY);
	filling(i,j);
	}
//color::direct(0);
//color::tohardware();
cleardevice();
getch();
BiosScreen(0x3);
}