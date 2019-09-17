/*
		       VESA256c.c
	Funkcje graficzne dla trybu 100H,101H i innych 256-scio kolorowych
	trybow zgodnych ze standartem VESA
*/
#include <DOS.H>
#include <stddef.h>
#include <values.h>
#include <mem.h>
#include <math.h>
#include "BIOSGRAF.h"
#include "VESA.hpp"
#include "topvga.h"

#define MEMSET _fmemset
#define MEMCOPY _fmemcpy

extern unsigned long int LINELEN;

inline void PlotPixelVESA( int x, int y)
{
if( x < 0L || y < 0L || x > MAXX || y> (long)MAXY ) return;

unsigned long int offset=((long)(LINELEN)*y)+x;
unsigned long int pom;
pom=(offset>>16) ;		         /* /FFFFh */
if(pom!=BANKINA)
	AssignBank(0,pom);
/* PUT PIXEL */
*( SCREEN+(offset-(pom<<16)) )=COLOR;	 /* %FFFFh */
}


void   far SetPixelVESA(long int xx , long int yy ,unsigned int color)
{
if( xx < 0L || yy < 0L || xx > MAXX ||yy> (long)MAXY ) return;
int x=xx;
int y=yy;
unsigned long int offset=((long)(LINELEN)*y)+x;
unsigned long int pom;
pom=(offset>>16) ;				 /* /FFFFh */
if(pom!=BANKINA)
	AssignBank(0,pom);
/* PUT PIXEL */
*( SCREEN+(offset-(pom<<16)))=color;	 /* %FFFFh */
}

static void PlotHoriz(int xx1,int yy1,int n)
{
unsigned long int offset;
unsigned long int pom;
POCZATEK:
offset=((long)(LINELEN)*yy1)+xx1;
pom=(offset>>16) ;				 /* /FFFFh */
if(pom!=BANKINA)
	AssignBank(0,pom);
offset=(offset-(pom<<16));
/* PUT PIXEL */
if(offset+n<=0x0000ffff)
	{
	MEMSET( SCREEN+offset,COLOR,n);
	}
	else
	{
	pom=0x0000ffff-offset;
	MEMSET( SCREEN+offset,COLOR,pom);
	xx1+=pom+1;
	n-=pom;
	goto POCZATEK;
	}
}

static void CopyHoriz(int xx1,int yy1,unsigned char far* s,int n)
{

unsigned long int offset;
unsigned long int pom;
POCZATEK:
offset=((long)(LINELEN)*yy1)+xx1;
pom=(offset>>16) ;				 /* /FFFFh */
if(pom!=BANKINA)
	AssignBank(0,pom);
offset=(offset-(pom<<16));
/* PUT PIXEL */
if(offset+n<=0x0000ffff)
	{
	linecopy( SCREEN+offset,s,n);
	}
	else
	{
	pom=0x0000ffff-offset;
	MEMCOPY( SCREEN+offset,s,pom);
	xx1+=pom+1;
	n-=pom;
	goto POCZATEK;
	}
}

void  far PutMapVESA(long int x1, long int y1,int n,int m,unsigned char far* bufor)
{
int yy1,i;
int xx1=x1;
int len;
//if(x1>=0)
  // len=(x1+m<=MAXX?m:MAXX-x1);
len=m;
for(i=0;i<n;i++)
	{
	yy1=y1+i;
	if(yy1>=0 && yy1<=MAXY)
		CopyHoriz( xx1,yy1 ,bufor ,len );
	bufor+=m;
	}
}


void  far CleardeviceVESA()
{
int i;
for(i=0;i<BANK_NUM;i++)
	{
	AssignBank(0,i);
	MEMSET( SCREEN , 0 , 0xFFFE);
	SCREEN[0xffff]=0;
	}
}

void  far FillBarVESA(long int x1, long int y1,long int x2, long int y2)
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

void  far ClearBarVESA(long int x1, long int y1,long int x2, long int y2)
{
int color=COLOR;
COLOR=0;
FillBarVESA(x1,y1,x2,y2);
COLOR=color;
}

void  far FillEllipseVESA(long int xxs,long int yys,long int aa,long int bb)
{
if(bb<=1){SetPixelVESA(xxs,yys,COLOR);return;}
int xs=xxs;
int ys=yys;
int b=(bb<=MAXINT?bb:MAXINT);
int a=(aa<=MAXINT?aa:MAXINT);
int y,x,xa,ya,xe;
long int y2;
unsigned long a2,b2;
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



void  far LineVESA( long int xp , long int yp ,long int xk , long int yk )
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

PlotPixelVESA(xxp,yyp);
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
		PlotPixelVESA(xxp,yyp);
		}
	}
   else /* dy>= dx */
	{
	p=2*dx-dy; c1=2*dx; c2=2*(dx-dy);
	while( yyp!=yyk)
		{
		yyp+=y;
		if( p<0 ) p+=c1; else { xxp+=x; p+=c2; }
		PlotPixelVESA(xxp,yyp);
		}
	}
}


void  far EllipseVESA(long int xxs,long int yys,long int bb,long int aa)
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
PlotPixelVESA( xs+xi, ys+yi );PlotPixelVESA( xs-xi, ys-yi );
PlotPixelVESA( xs+xi, ys-yi );PlotPixelVESA( xs-xi, ys+yi );

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
}

void  far FillCircleVESA(long int xxs,long int yys,long int bb)
{
long int aa=bb*XRATIO/YRATIO;
if(aa==0) aa=bb;
FillEllipseVESA(xxs,yys,bb,aa);
}

void  far CircleVESA(long int xxs,long int yys,long int bb)
/* narysowanie okregu  */
{
EllipseVESA(xxs,yys,bb,bb*XRATIO/YRATIO);
}
