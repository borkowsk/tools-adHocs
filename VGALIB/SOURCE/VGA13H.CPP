/*
			VGA13h.c
	Funkcje graficzne dla trybu 13H  karty VGA via A000 adress
*/
#include <DOS.H>
#include <stddef.h>
#include <values.h>
#include <mem.h>
#include <math.h>

#include "BIOSGRAF.h"
#include "TOPVGA.H"


#define MEMSET _fmemset
#define MEMCOPY _fmemcpy

#define OFFSET13H( _X_ , _Y_ )  ((LINELEN*(_Y_))+(_X_))

void   far SetPixel13H(long int x , long int y ,unsigned int color)
{
if( x < 0L || y < 0L || x > MAXX || y> MAXY ) return;
*( SCREEN+OFFSET13H( x , y ) )=color;
}

void  far Cleardevice13H()
{
MEMSET( SCREEN , 0 , (MAXX+1)*(MAXY+1));
}

void  far ClearBar13H(long int x1, long int y1,long int x2, long int y2)
{
register unsigned int xx1,yy1,yy2,  len ;
if( x2<x1 || y2<y1 || x2<0L || y2<0L || x1>MAXX || y1>MAXY ) return;
xx1 = ( x1>  0L ? x1:   0 );
yy1 = ( y1>  0L ? y1:   0 );
yy2 = ( y2<=MAXY ? y2: MAXY );
len = ( x2<=MAXX ? x2: MAXX  )-xx1+1;

while(yy1<=yy2)
	{
	MEMSET( SCREEN+OFFSET13H( xx1,yy1 ) , 0 , len );
	yy1++;
	}
}

void  far FillBar13H(long int x1, long int y1,long int x2, long int y2)
{
register unsigned int xx1,yy1,yy2,  len ;
if( x2<x1 || y2<y1 || x2<0L || y2<0L || x1>MAXX || y1>MAXY ) return;
xx1 = ( x1>  0L ? x1:   0 );
yy1 = ( y1>  0L ? y1:   0 );
yy2 = ( y2<=MAXY ? y2: MAXY );
len = ( x2<=MAXX ? x2: MAXX )-xx1+1;

while(yy1<=yy2)
	{
	MEMSET( SCREEN+OFFSET13H( xx1,yy1 ) , COLOR , len );
	yy1++;
	}
}

/*
void  far GetMap13H(long int x1, long int y1,int n,int m,unsigned char far* bufor)
{

}
*/

void  far PutMap13H(long int x1, long int y1,int n,int m,unsigned char far* bufor)
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
		linecopy( SCREEN+OFFSET13H( xx1,yy1 ),bufor ,len );
	bufor+=m;
	}
}


void far FillEllipse13H(long int xxs,long int yys,long int aa,long int bb)
{
if(bb<=1)
    if(xxs>=0 && xxs<=MAXX && yys>=0 && yys<=MAXY )
	{SetPixel13H(xxs,yys,COLOR);
	 return;}
int xs=xxs;
int ys=yys;
unsigned long int y2,a2,b2;
int b=(bb<=MAXINT?bb:MAXINT);
int a=(aa<=MAXINT?aa:MAXINT);
int y,x,xa,ya,xe;
b2=(unsigned long int)b*b;
a2=(unsigned long int)a*a;
for(y=0;y<b;y++)
	{
	y2=(unsigned long int)y*y;
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
		MEMSET( SCREEN+OFFSET13H(xa,ya) , COLOR , x );
	ya=ys-y;
	if(ya>=0 && ya<=MAXY)
		MEMSET( SCREEN+OFFSET13H(xa,ya) , COLOR , x );
	}
}

void  far Line13H( long int xp , long int yp ,long int xk , long int yk )
/* narysowanie lini w dowolnym kierunku algorytmen Bresenhama */
{
register int xxp;/* zastepniki parametrow */
register int yyp;
int	 xxk,yyk;
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

if(xxp>=0 && xxp<=MAXX && yyp>=0 && yyp<=MAXY )
	*( SCREEN+OFFSET13H( xxp , yyp ) )=COLOR;
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
		if(xxp>=0 && xxp<=MAXX && yyp>=0 && yyp<=MAXY )
			*( SCREEN+OFFSET13H( xxp , yyp ) )=COLOR;
		}
	}
   else /* dy>= dx */
	{
	p=2*dx-dy; c1=2*dx; c2=2*(dx-dy);
	while( yyp!=yyk)
		{
		yyp+=y;
		if( p<0 ) p+=c1; else { xxp+=x; p+=c2; }
		if(xxp>=0 && xxp<=MAXX && yyp>=0 && yyp<=MAXY )
			*( SCREEN+OFFSET13H( xxp , yyp ) )=COLOR;
		}
	}
}



void  far Ellipse13H(long int xxs,long int yys,long int bb,long int aa)
{
register int xxp,yyp;
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
xxp=xs+xi;yyp= ys+yi;
if(xxp>=0 && xxp<=MAXX && yyp>=0 && yyp<=MAXY )
	*( SCREEN+OFFSET13H( xxp , yyp ) )=COLOR;

xxp=xs-xi;yyp= ys-yi;
if(xxp>=0 && xxp<=MAXX && yyp>=0 && yyp<=MAXY )
	*( SCREEN+OFFSET13H( xxp , yyp ) )=COLOR;

xxp=xs+xi;yyp= ys-yi;
if(xxp>=0 && xxp<=MAXX && yyp>=0 && yyp<=MAXY )
	*( SCREEN+OFFSET13H( xxp , yyp ) )=COLOR;

xxp=xs-xi;yyp= ys+yi;
if(xxp>=0 && xxp<=MAXX && yyp>=0 && yyp<=MAXY )
	*( SCREEN+OFFSET13H( xxp , yyp ) )=COLOR;

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

void  far FillCircle13H(long int xxs,long int yys,long int bb)
{
long int aa=bb*XRATIO/YRATIO;
if(aa==0) aa=bb;
FillEllipse13H(xxs,yys,bb,aa);
}

void  far Circle13H(long int xxs,long int yys,long int bb)
/* narysowanie okregu  */
{
Ellipse13H(xxs,yys,bb,bb*833/1000);
}