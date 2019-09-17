/*
			TopVGA.c
	Funkcje graficzne dla trybu 5EH karty TopVGA  (via PROA register)
*/
#include <DOS.H>
#include <stddef.h>
#include <values.h>
#include <mem.h>
#include <math.h>

#include "BIOSGRAF.h"
#include "topvga.h"

#define MEMSET _fmemset


/*     MOV  DX,3CE         ; GRAPHICS CONTROLLER INDEX PORT ADDRESS
       MOV  AL,0F          ; PR5 INDEX VALUE
       MOV  AH,5           ; THE VALUE 5 "UNLOCKS" PR0A TO PR4
       OUT  DX,AX          ; UNLOCK
*/

inline void UnlockPROA()
{ outport( 0x3CE , 0x050F );};

/*     MOV  AL,0F          ; PR5 INDEX VALUE
       MOV  AH,0           ; VALUES OTHER THAN 5 "LOCK" PR0A TO PR4
       OUT  DX,AX          ; RELOCK
*/

inline void RelockPROA()
{ /*outport( 0x3CE , 0x000F );*/}// Nie dziala jak trzeba z biosem znakowym

/*     MOV  AL,0A          ; PR0B INDEX VALUE
       MOV  AH,val         ; DESIRED SETTING val
       OUT  DX,AX          ; SET PR0B
*/

inline void OutPROB( unsigned char _val_ )
{ outport( 0x3CE , ((_val_)<<8)|0x0A );}

/*     MOV  AL,09          ; PR0A INDEX VALUE
       MOV  AH,val         ; DESIRED SETTING val
       OUT  DX,AX          ; SET PR0B
*/

inline void OutPROA( unsigned char _val_ )
{ outport( 0x3CE , ((_val_)<<8)|0x09 );}
/*
inline void SetPROB( unsigned char val )
{
UnlockPROB;
OutPROB( val );
RelockPROB;
}
*/
inline void SetPROA( unsigned char val )
{
UnlockPROA();
OutPROA( val );
RelockPROA();
}
extern unsigned long int LINELEN;
#define OFFSETPRA( _X_ , _Y_ )  (((long)(LINELEN)*(_Y_))+(_X_))



void   far SetPixelPRA(long int x , long int y ,unsigned int color)
{
register unsigned int pom;
if( x < 0L || y < 0L || x > MAXX || y> MAXY ) return;
unsigned long int offset=OFFSETPRA( x , y );
UnlockPROA();
OutPROA( pom=(offset>>12) );		 /* /4096 */
*( SCREEN+(offset-(pom<<12)) )=color;	 /* %4096 */

OutPROA( 0x0 );
RelockPROA();
}

void  far CleardevicePRA()
{
unsigned int j;

UnlockPROA();
for( j=0 ; j<64 ; j+=8 )
	{
	OutPROA( j );
	MEMSET( SCREEN , 0 , 0x8000 );
	}

OutPROA( 0x0 );
RelockPROA();
}



void  far ClearBarPRA(long int x1, long int y1,long int x2, long int y2)
{
unsigned long int pom;
unsigned int yy1,yy2,len;

if( x2<x1 || y2<y1 || x2<0L || y2<0L || x1>MAXX || y1>MAXY ) return;

 x1 = ( x1>  0L ? x1:   0 );
 yy1=y1 = ( y1>  0L ? y1:   0 );
 y2 = ( y2<MAXY ? y2: MAXY );
 x2 = ( x2<MAXX ? x2: MAXX );

len = x2-x1;
UnlockPROA();
do{
unsigned long int offset=OFFSETPRA( x1 , (long)yy1 );
SetPROA( pom=(offset>>12) ); /* /4096*/
pom<<=12; /* *4096 */

if( OFFSETPRA( x2 , y2 )-pom>0xFF00)
	{
	yy2=((long)pom+0xFF00)/(long)MAXX;
	if(yy2>y2) yy2=y2;
	}
	else
	yy2=y2;

while(yy1<=yy2)
	{
	MEMSET( SCREEN+(offset-pom) , 0 , len );
	yy1++;
	offset=OFFSETPRA( x1 , (long)yy1 );
	}

}while(yy2<y2);

OutPROA( 0 );
RelockPROA();
}


void  far FillBarPRA(long int x1, long int y1,long int x2, long int y2)
{
unsigned long int pom;
unsigned int yy1,yy2,len;

if( x2<x1 || y2<y1 || x2<0L || y2<0L || x1>MAXX || y1>MAXY ) return;

 x1 = ( x1>  0L ? x1:   0 );
 yy1=y1 = ( y1>  0L ? y1:   0 );
 y2 = ( y2<MAXY ? y2: MAXY );
 x2 = ( x2<MAXX ? x2: MAXX );
len = x2-x1;

UnlockPROA();
do{
unsigned long int offset=OFFSETPRA( x1 , (long)yy1 );
SetPROA( pom=(offset>>12) ); /* /4096*/
pom<<=12; /* *4096 */

if( OFFSETPRA( x2 , y2 )-pom>0xFF00)
	{
	yy2=((long)pom+0xFF00)/(long)MAXX;
	if(yy2>y2) yy2=y2;
	}
	else
	yy2=y2;

while(yy1<=yy2)
	{
	MEMSET( SCREEN+(offset-pom) , COLOR , len );
	yy1++;
	offset=OFFSETPRA( x1 , (long)yy1 );
	}

}while(yy2<y2);

OutPROA( 0 );
RelockPROA();
}

void far FillEllipsePRA(long int xxs,long int yys,long int aa,long int bb)
{
unsigned long int offset=0L,pom;
int xs=xxs;
int ys=yys;
unsigned long int y2,a2,b2;
int b=(bb<=MAXINT?bb:MAXINT);
int a=(aa<=MAXINT?aa:MAXINT);
int y,x,xa,ya,xe;
b2=(unsigned long int)b*b;
a2=(unsigned long int)a*a;
UnlockPROA();
for(y=0;y<b;y++)
	{
	y2=(unsigned long int)y*y;
	x=isqrt(((double)a2*b2-(double)a2*y2)/b2);
	xe=xs+x;
	if(xe>(int)MAXX ) xe=MAXX;
	xa=xs-x;
	if(xa<0) xa=0;
	x=xe-xa;

	if((x=xe-xa+1)<=0) continue;

	ya=ys+y;
	if(ya>=0 && ya<=MAXY)
		{
		offset=OFFSETPRA( xa , ya );
		OutPROA( pom=(offset>>12) );	 /* /4096 */
						 /* %4096 */
		MEMSET( SCREEN+(offset-(pom<<12)) , COLOR , x );
		}
	ya=ys-y;
	if(ya>=0 && ya<=MAXY)
		{
		offset=OFFSETPRA( xa , ya );
		OutPROA( pom=(offset>>12) );	 /* /4096 */
						 /* %4096 */
		MEMSET( SCREEN+(offset-(pom<<12)) , COLOR , x );
		}
	}
OutPROA( 0x0 );
RelockPROA();
}

inline void PlotPixelPRA( int x, int y)
{
if( x < 0L || y < 0L || x > MAXX || y> MAXY ) return;
unsigned int pom;
unsigned long int offset=OFFSETPRA( x , y );
OutPROA( pom=(offset>>12) );		 /* /4096 */
*( SCREEN+(offset-(pom<<12)) )=COLOR;	 /* %4096 */
}

void  far LinePRA( long int xp , long int yp ,long int xk , long int yk )
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

UnlockPROA();
PlotPixelPRA(xxp,yyp);
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
		PlotPixelPRA(xxp,yyp);
		}
	}
   else /* dy>= dx */
	{
	p=2*dx-dy; c1=2*dx; c2=2*(dx-dy);
	while( yyp!=yyk)
		{
		yyp+=y;
		if( p<0 ) p+=c1; else { xxp+=x; p+=c2; }
		PlotPixelPRA(xxp,yyp);
		}
	}
OutPROA( 0x0 );
RelockPROA();
}




void  far EllipsePRA(long int xxs,long int yys,long int bb,long int aa)
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

UnlockPROA();
while( yi>=0 )
{
PlotPixelPRA( xs+xi, ys+yi );PlotPixelPRA( xs-xi, ys-yi );
PlotPixelPRA( xs+xi, ys-yi );PlotPixelPRA( xs-xi, ys+yi );

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

/*  Przywrocenie standartowych parametrow */
OutPROA( 0x0 );
RelockPROA();
}

void  far FillCirclePRA(long int xxs,long int yys,long int bb)
{
long int aa=bb*XRATIO/YRATIO;
if(aa==0) aa=bb;
FillEllipsePRA(xxs,yys,bb,aa);
}


void  far CirclePRA(long int xxs,long int yys,long int bb)
/* narysowanie okregu  */
{
EllipsePRA(xxs,yys,bb,bb*XRATIO/YRATIO);
}