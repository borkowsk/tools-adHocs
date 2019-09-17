
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

main()
{
int mode,CUBESIZE=1;
#define COLORJMP (64/CUBESIZE)
int i,j,k,n=64,skokx,skoky;
color c;
char *end,tab[30];
printf("Podaj nr trybu graficznego:");
gets(tab);
mode=strtol( tab, &end , 0 );
printf("Podaj  rozmiar \"boku\" kostki kolorow:");
gets(tab);
CUBESIZE=strtol( tab, &end , 0 );
BiosScreen(mode);
color::fromhardware();
color::direct(0);
for(i=0;i<32;i++)
	{
	COLOR=i;/* Pula stalych kolorow */
	BiosLine(i,0,i,MAXY);
	}
for(i=32;i<256;i++)
	{
//	c.alloc(i+32,i+32,i+32);
	COLOR=i;
	BiosLine(i,0,i,MAXY);
//	c.free();//nie zwalniac !
	}

getch();
color::tohardware();
getch();
color::direct(1);
color::cube(CUBESIZE);
getch();
BiosCleardevice();
n=64;
for(i=0;i<n;i++)
	for(j=0;j<n;j++)
		{
		c.getcube(i,j,COLORJMP);
		BiosSetPixel(MAXX-i-64,j,c);
		}
for(i=0;i<n;i++)
	for(j=0;j<n;j++)
		{
		c.getcube(i,COLORJMP,j);
		BiosSetPixel(MAXX-i-64,j+64,c);
		}
for(i=0;i<n;i++)
	for(j=0;j<n;j++)
		{
		c.getcube(COLORJMP,i,j);
		BiosSetPixel(MAXX-i-64,j+128,c);
		}

for(i=0;i<n;i++)
	for(j=0;j<n;j++)
		{
		c.getnear(i,j,COLORJMP);
		BiosSetPixel(MAXX-i,j,c);
		}
for(i=0;i<n;i++)
	for(j=0;j<n;j++)
		{
		c.getnear(i,COLORJMP,j);
		BiosSetPixel(MAXX-i,j+64,c);
		}
for(i=0;i<n;i++)
	for(j=0;j<n;j++)
		{
		c.getnear(COLORJMP,i,j);
		BiosSetPixel(MAXX-i,j+128,c);
		}

for(i=0;i<n;i+=2)
	{
	skokx=i*(MAXX-64)/64;
	skoky=i*(MAXY-64)/64;
	for(j=0;j<n;j++)
		for(k=0;k<n;k++)
			{
			c.getcube(i,j,k);
			BiosSetPixel(j+skokx,k+skoky,c);
			}
	}
getch();
BiosScreen(0x3);
}