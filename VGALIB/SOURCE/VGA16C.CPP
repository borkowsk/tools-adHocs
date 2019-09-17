/*
		       VGA16c.c
	Funkcje graficzne dla trybu 58H i innych 16-sto kolorowych
*/
#include <DOS.H>
#include <stddef.h>
#include <values.h>
#include <mem.h>
#include <math.h>

#include "BIOSGRAF.h"
#include "topvga.h"

#define MEMSET _fmemset
extern unsigned long int LINELEN;

void   far SetPixel16C(long int x , long int y ,unsigned int color)
{
if( x < 0L || y < 0L || x > MAXX || y> (long)MAXY ) return;

unsigned long int offset=((long)(LINELEN)*y)+x;
unsigned char far * baddr=SCREEN+(offset>>3);
unsigned char _for_read;
unsigned char mask=0x80>>(offset-((unsigned long)(offset>>3)<<3));

/* PUT PIXEL */
outp(0x3CE, 8);
outp(0x3CF, mask); /* O ktory bit chodzi */
_for_read=*(baddr);	   /*  ???? */
*(baddr)=0;	   /* Czyszczenie punktu */

outp(0x3C4, 2);
outp(0x3C5, color);/* Ustawienie koloru */
*(baddr)=mask;     /* Ustawienie punktu */

/*  Przywrocenie standartowych parametrow */
outp(0x3C4, 2);
outp(0x3C5, 0xF); /* kolor 16 */
outp(0x3CE, 8);
outp(0x3CF, 0xFF);/* Maska - Full */
}

inline void PlotPixel16C( int x, int y)
{
if( x < 0L || y < 0L || x > MAXX || y> (long)MAXY ) return;

unsigned long int offset=((long)(LINELEN)*y)+x;
unsigned char far * baddr=SCREEN+(offset>>3);
unsigned char lay;
unsigned char mask=0x80>>(offset-((unsigned long)(offset>>3)<<3));

/* PUT PIXEL */
outp(0x3CE, 8);
outp(0x3CF, mask); /* O ktory bit chodzi */
lay=*(baddr);	   /*  ???? */
*(baddr)=0;	   /* Czyszczenie punktu */

outp(0x3C4, 2);
outp(0x3C5, COLOR);/* Ustawienie koloru */
*(baddr)=mask;     /* Ustawienie punktu */
outp(0x3C4, 2);
outp(0x3C5, 0xF); /* kolor 16 */
}

static void PlotHoriz(int xx1,int yy1,int n)
{
unsigned char far * baddr;
unsigned char far * Eaddr;
unsigned char lay;
unsigned char mask;

unsigned long int offset0=((long)(LINELEN)*yy1)+xx1;
unsigned long int offset1=offset0+n;
baddr=SCREEN+(offset0>>3);/* Adres poczatkowy */
Eaddr=SCREEN+(offset1>>3);/* Adres koncowy */

if(baddr==Eaddr)
	{
	mask=0xFF>>(offset0-((unsigned long)(offset0>>3)<<3));
	lay=0xFF<<(8-(/*1*/+offset1-((unsigned long)(offset1>>3)<<3)));
	mask=mask&lay;
	goto ONEBAYT;
	}
	else
	mask=0xFF>>(offset0-((unsigned long)(offset0>>3)<<3));

outp(0x3CE, 8);
outp(0x3CF, mask); /* O ktory bit chodzi */
lay=*(baddr);	   /*  ???? */
*(baddr)=0;	   /* Czyszczenie punktu */

outp(0x3C4, 2);
outp(0x3C5,COLOR); /* Ustawienie koloru */
*(baddr)=mask;     /* Ustawienie punktu */

/* STANDART */
outp(0x3C4, 2);
outp(0x3C5, 0xF); /* kolor 16 */
outp(0x3CE, 8);
outp(0x3CF, 0xFF);/* Maska - Full */
mask=0xFF;

while(++baddr<Eaddr)
		{
		/* PUT PIXELs */
		lay=*(baddr);	   /*  ???? */
		*(baddr)=0;	   /* Czyszczenie punktow */
		outp(0x3C4, 2);
		outp(0x3C5,COLOR); /* Ustawienie koloru */
		*(baddr)=mask;     /* Ustawienie punktow */
		/* STANDART */
		outp(0x3C4, 2);
		outp(0x3C5, 0xF); /* kolor 16 */
		}
mask=0xFF<<(8-(/*1*/+offset1-((unsigned long)(offset1>>3)<<3)));

ONEBAYT:
outp(0x3CE, 8);
outp(0x3CF, mask); /* O ktory bit chodzi */
lay=*(baddr);	   /*  ???? */
*(baddr)=0;	   /* Czyszczenie punktu */

outp(0x3C4, 2);
outp(0x3C5,COLOR); /* Ustawienie koloru */
*(baddr)=mask;     /* Ustawienie punktu */

/* STANDART */
outp(0x3C4, 2);
outp(0x3C5, 0xF); /* kolor 16 */
outp(0x3CE, 8);
outp(0x3CF, 0xFF);/* Maska - Full */

}



void  far Cleardevice16C()
{
/* prowizoryczny - nie tak jak trzeba*/
MEMSET( SCREEN , 0 , 0xFFFE);
}

void  far FillBar16C(long int x1, long int y1,long int x2, long int y2)
{
unsigned int xx1,xx2, i ;
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

void  far ClearBar16C(long int x1, long int y1,long int x2, long int y2)
{
int color=COLOR;
COLOR=0;
FillBar16C(x1,y1,x2,y2);
COLOR=color;
}

void  far FillEllipse16C(long int xxs,long int yys,long int aa,long int bb)
{
if(bb<=1){SetPixel16C(xxs,yys,COLOR);return;}
int xs=xxs;
int ys=yys;
long int y2;
unsigned long a2,b2;
int b=(bb<=MAXINT?bb:MAXINT);
int a=(aa<=MAXINT?aa:MAXINT);
int y,x,xa,ya,xe;
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



void  far Line16C( long int xp , long int yp ,long int xk , long int yk )
/* narysowanie lini w dowolnym kierunku algorytmen Bresenhama */
{
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

PlotPixel16C(xxp,yyp);
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
		PlotPixel16C(xxp,yyp);
		}
	}
   else /* dy>= dx */
	{
	p=2*dx-dy; c1=2*dx; c2=2*(dx-dy);
	while( yyp!=yyk)
		{
		yyp+=y;
		if( p<0 ) p+=c1; else { xxp+=x; p+=c2; }
		PlotPixel16C(xxp,yyp);
		}
	}
/*  Przywrocenie standartowych parametrow */
outp(0x3C4, 2);
outp(0x3C5, 0xF); /* kolor 16 */

outp(0x3CE, 8);
outp(0x3CF, 0xFF);/* Maska - Full */
}


void  far Ellipse16C(long int xxs,long int yys,long int bb,long int aa)
{

register int xs=xxs;
register int ys=yys;
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
PlotPixel16C( xs+xi, ys+yi );PlotPixel16C( xs-xi, ys-yi );
PlotPixel16C( xs+xi, ys-yi );PlotPixel16C( xs-xi, ys+yi );

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
//continue;

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
/*  Przywrocenie standartowych parametrow */
outp(0x3C4, 2);
outp(0x3C5, 0xF); /* kolor 16 */

outp(0x3CE, 8);
outp(0x3CF, 0xFF);/* Maska - Full */
}

void  far FillCircle16C(long int xxs,long int yys,long int bb)
{
long int aa=bb*XRATIO/YRATIO;
if(aa==0) aa=bb;
FillEllipse16C(xxs,yys,bb,aa);
}


void  far Circle16C(long int xxs,long int yys,long int bb)
/* narysowanie okregu  */
{
Ellipse16C(xxs,yys,bb,bb*XRATIO/YRATIO);
}
