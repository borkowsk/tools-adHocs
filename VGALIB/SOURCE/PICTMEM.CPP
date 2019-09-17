/*
Workspace User Interface - podstawowe metody klasy mem_picture
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <values.h>
#include <math.h>
#include <mem.h>
#include <conio.h>
#include <dos.h>

#include "biosgraf.h"
#include "color.hpp"
#include "picture.hpp"
#include "fonts.hpp"

/* Portable section */
#define OFFSET( _X_ , _Y_ )  ((LINELEN*(_Y_))+(_X_))
inline void MEMMOVE(void far* d,void far* s,unsigned len)
{
movmem(s,d,len);
}

void mem_picture::PlotPixel(pixsize_t x,pixsize_t y)
{
*( GetFrameBuferAddr()+OFFSET( x , y ) )=CUR_COLOR;
}

pixpointer mem_picture::CalcPixelAddr(pixsize_t& x,pixsize_t& y) const
{
pixpointer pom= GetFrameBuferAddr()+OFFSET( x , y );// Calculate adres
x=MAXX;y=MAXY;	// Return borders for use this adress
return pom;     // Return adress
}
#undef OFFSET

int mem_picture::GetPicture(const picture& sour ,pixco_t x1 ,pixco_t y1 )
 // Copy from another picture
{
if( x1<0 || y1<0 || x1>sour.GetMaxX() || y1>sour.GetMaxY())
		 return 0;
pixco_t yy1,xx1=x1;
pixco_t len=GetLineLen();
pixco_t n=MAXY;
for(int i=0;i<=n;i++)
	{
	yy1=y1+i;
	if(yy1>=0 && yy1<=sour.GetMaxY())
		{
		pixsize_t xp=0,yp=i;
		sour.GetLine(CalcPixelAddr(xp,yp), xx1,yy1,len);
		}
	}
return 1;
}


int mem_picture::ShiftUp(pixco_t s)
{
if(s<=0) return 0;
if(s>=MAXY) return Clear();
pixsize_t zero=0,shift=s;//x i y wywolaniu CalcPixelAddr moga byc zmienione (referencja)
MEMCPY(GetFrameBuferAddr(),CalcPixelAddr(zero,shift),(MAXY-s)*LINELEN);
return 1;
}

int mem_picture::ShiftDown(pixco_t s)
{
if(s<=0) return 0;
if(s>=MAXY) return Clear();
pixsize_t zero=0,shift=s;//x i y wywolaniu CalcPixelAddr moga byc zmienione (referencja)
MEMMOVE(CalcPixelAddr(zero,shift),GetFrameBuferAddr(),(MAXY-s+1)*LINELEN);
return 1;
}

int mem_picture::ShiftLine(pixco_t x,pixco_t  y,pixco_t shval)
 // Shift line
{
pixpointer lineaddr;
if(y<0 || y>MAXY || x>MAXX || x+shval>MAXX ) // Bledne dane
	return 0;
if(shval==0) return 1;
if(x<0) x=0;
pixsize_t len=LINELEN-abs(shval)-x;
	{pixsize_t xx=x,yy=y;// Niepotrzebne dane o zakresie - a i typ sie nie zgadza
lineaddr=CalcPixelAddr(xx,yy);		}
if(shval>0)
	MEMMOVE(lineaddr+shval,lineaddr,len);
	else
	MEMMOVE(lineaddr,lineaddr+abs(shval),len);
return 1;
}

