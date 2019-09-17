/*
WorkSpace User Interface - font rastrowy o szerokosci 8-bitow
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <values.h>
#include <math.h>
#include <dos.h>
#include <conio.h>

#include "biosgraf.h"
#include "color.hpp"
#include "picture.hpp"
#include "screens.hpp"
#include "fonts.hpp"

pixsize_t font8bit::TextWidth(const char* t)
	{ return strlen(t)*8; }
pixsize_t font8bit::TextHight(const char* )
	{ return hight; }
int 	font8bit::OutTextXY(picture& p,pixco_t x,pixco_t y,const char* t)
{
int i,j;
char m;
pixel tab[8];
color_handle kolor=p.GetColor();
color_handle bacground=p.GetBackground();
while(*t!='\0')
	{
	for(i=0;i<hight;i++)
	   {
	   m=mattrix[(*t)*hight+i];
	   for(j=0;j<8;j++)
		{
		if((m&(128>>j))!=0)
				tab[j]=kolor;
				else
				tab[j]=bacground;
		}
	    p.PutLine(x,y+i,8,tab);
	    }
	t++;x+=8;
	}
return 1;
}