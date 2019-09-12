/*
BIOS-owa obsluga karty graficznej - procedury sredniego poziomu
*/
#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include <DOS.h>
#include <math.h>
#include <conio.h>
#include "BIOSGRAF.h"
#pragma options( -r- )

static void PlotPixelR( int _xx, int _yy )
{
if(_xx<0 ||_yy<0 || _xx>MAXX || _yy>MAXY ) return;
_CX=_xx;_DX=_yy;_BH=PAGE;
_AL=COLOR;_AH=0x0C;/* Funkcja 12 */
geninterrupt(0x10);
}

static void PlotHoriz(int xx1,int yy1,int n)
{
unsigned int axb;
if(n==0) return;
_AL=COLOR;
_AH=0x0C;/* Funkcja 12 */
axb=_AX;
_DX=yy1;
_BH=PAGE;
for(int i=xx1+n-1;i>=xx1;i--)
	{
	_CX= i;
	_AX=axb;
	Int10H();
	}
}



void  far BiosFillBar(long int x1, long int y1,long int x2, long int y2)
{
register unsigned int xx1,xx2, i ;
unsigned int yy1,yy2;

if( x2<x1 || y2<y1 || x2<0L || y2<0L || x1>MAXX || y1>MAXY ) return;
xx1 = ( x1>  0L ? x1:   0 );
yy1 = ( y1>  0L ? y1:   0 );
yy2 = ( y2<MAXY ? y2: MAXY );
xx2 = ( x2<MAXX ? x2: MAXX );
if((xx2=xx2-xx1+1)<=0) return;
while(yy1<=yy2)
	{
	PlotHoriz(xx1,yy1,xx2);
	yy1++;
	}
}

void  far BiosClearBar(long int x1, long int y1,long int x2, long int y2)
{
int color=COLOR;
COLOR=0;
BiosFillBar(x1,y1,x2,y2);
COLOR=color;
}
void  far BiosFillCircle(long int xxs,long int yys,long int bb)
{
#pragma options(-r-)
int xs=xxs;
int ys=yys;
int b=(bb<=MAXINT?bb:MAXINT);
int y,x,xa,ya,xe,a;
long int y2;
unsigned long a2,b2;
a=b;
b=(long int)b*XRATIO/YRATIO;
b2=(long int)b*b;
a2=(long int)a*a;
for(y=0;y<b;y++)
	{
	y2=(long int)y*y;
	x=isqrt(( (double)a2*b2-(double)a2*y2 )/b2);
	xe=xs+x;
	if(xe>(int)MAXX )
		xe=MAXX;
	xa=xs-x;
	if(xa<0)
		xa=0;
	if((x=xe-xa+1)<=0) continue;

	ya=ys+y;
	if(ya>=0 && ya<=MAXY)
		PlotHoriz(xa,ya,x);

	ya=ys-y;
	if(ya>=0 && ya<=MAXY)
		PlotHoriz(xa,ya,x);

	}
}

void  far BiosLine( long int xp , long int yp ,long int xk , long int yk )
/* narysowanie lini w dowolnym kierunku algorytmen Bresenhama */
{
#pragma options -rd
int xxp,xxk;/* zastepniki parametrow */
int yyp,yyk;
int dx,dy,x,y,p,c1,c2;/* zmienne pomocnicze */

if(xp<-MAXINT) xxp=-MAXINT;
   else if(xp>MAXINT) xxp=MAXINT;
	    else   xxp=xp;
if(xk<-MAXINT) xxk=-MAXINT;
   else if(xk>MAXINT) xxk=MAXINT;
	    else   xxk=xk;
if(yp<-MAXINT) yyp=-MAXINT;
   else if(yp>MAXINT) yyp=MAXINT;
	    else   yyp=yp;
if(yk<-MAXINT) yyk=-MAXINT;
   else if(yk>MAXINT) yyk=MAXINT;
	    else   yyk=yk;

dx=abs(xxk-xxp); dy=abs(yyk-yyp);

PlotPixelR(xxp,yyp);
dx=abs(xxk-xxp); dy=abs(yyk-yyp);
if(xxk>xxp) x=1;
	else x=-1;
if(yyk>yyp) y=1;
	else y=-1;
if( dx>dy )
	{
	p=2*dy-dx; c1=2*dy; c2=2*(dy-dx);
	while( xxp!=xxk )
		{
		xxp+=x;
		if(p<0) p+=c1 ; else { yyp+=y; p+=c2; }
		PlotPixelR( xxp, yyp );
		}
	}
   else /* dy>= dx */
	{
	p=2*dx-dy; c1=2*dx; c2=2*(dx-dy);
	while( yyp!=yyk)
		{
		yyp+=y;
		if( p<0 ) p+=c1; else { xxp+=x; p+=c2; }
		PlotPixelR( xxp, yyp );
		}
	}
}

void  far BiosCircle(long int xxs,long int yys,long int bb)
/* narysowanie okregu  */
{
BiosEllipse(xxs,yys,bb,bb*XRATIO/YRATIO);
}

void  far BiosEllipse(long int xxs,long int yys,long int bb,long int aa)
{
#pragma options -r-
int xs=xxs;
int ys=yys;
long int yi=aa;

long int xi=0;
long int a2=aa*aa;
long int b2=bb*bb;

long int a2s=a2;
long int b2s=b2*(2*yi+1);
long int DELTAi=2*(1-bb);

if(aa==0 || bb==0) return;

while( yi>=0 )
{
PlotPixelR( xs+xi, ys+yi );PlotPixelR( xs-xi, ys-yi );
PlotPixelR( xs+xi, ys-yi );PlotPixelR( xs-xi, ys+yi );

if(DELTAi<0L) goto _1300;
   else {if(DELTAi==0L)
		goto _1240;
	  else
		goto _1180; }

_1180:			 /* decyzja */
if( (DELTAi+DELTAi-a2s)<=0L )
		goto _1240;
	else
		goto _1380;
continue;

_1240: 			/* krok ukosny */
xi++;
yi--;
a2s+=a2+a2;
b2s-=(b2+b2);
DELTAi+=(a2s-b2s);
continue;

_1300:			/* decyzja 2 */
if((DELTAi+DELTAi+b2s)>0L) goto _1240;
xi++;
a2s+=a2+a2;
DELTAi+=a2s;
continue;

_1380:			/* krok poziomy */
yi--;
b2s-=(b2+b2);
DELTAi-=b2s;
}

}

void  far BiosScrollBar(long int x1, long int y1,long int x2, long int y2 ,int lines)
{
#pragma options -r-
unsigned char xx1,xx2,yy1,yy2;
if( x2<0L || y2<0L || x1>TMAXX || y1>TMAXY ) return;
xx1 = ( x1>  0L ? x1:   0 );
yy1 = ( y1>  0L ? y1:   0 );
yy2 = ( y2<TMAXY ? y2: TMAXY );
xx2 = ( x2<TMAXX ? x2: TMAXX );
{
unsigned char pom;
if(xx1>xx2){ pom=xx1; xx1=xx2; xx2=pom; }
if(yy1>yy2){ pom=yy1; yy1=yy2; yy2=pom; }
}
_CH=yy1;
_CL=xx1;
_DH=yy2;
_DL=xx2;
_BH=COLOR;
_AL=lines;
_AH=0x6;
geninterrupt( 0x10 );
}

