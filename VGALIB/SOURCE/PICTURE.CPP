/*
Workspace User Interface - podstawowe metody klasy picture 
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

extern char defaultfonttable[]; // Tablica fontow 
extern font8bit DefaultFont;    // Podstawowy font wewnetrzny
extern palette  DefaultPalette; // Podstawowa paleta


int picture::SetCopyMode(copymode m)
// Set mode for copy pixel to this picture
{
 return 0;
}

pixpointer picture::PixelAddr(pixco_t& x,pixco_t& y) const
{
if(x>=0 && x<=MAXX && y>=0 && y<=MAXY)
	{
	pixsize_t xx=(pixsize_t)x,yy=(pixsize_t)y;
	pixpointer pom=CalcPixelAddr(xx,yy);
	x=xx; y=yy;/* Wartosci powrotne */
	return pom;
	}
	else
	return NULL;
}


int picture::FillLine(pixco_t xx,pixco_t yy,pixco_t n)
{
pixpointer addr;
pixsize_t  x,y,xs,ys,pom;
if(  xx>MAXX || yy<0 || yy>MAXY || (xx+n)<0) /* Sprawdzanie */
	return 0;
if( xx < 0 )                               /* Obcinanie przed 0 */
	{ n-=abs(xx); xx=0;}
if( n<=0  )
	return 0;
if( xx+n>MAXX )				  /* Obcinanie za MAXX */
	n=MAXX-xx+1;

xs=x=(pixsize_t) xx;
ys=y=(pixsize_t) yy;
JESZCZERAZ:
addr=CalcPixelAddr(xs,ys); /* Zmienia wartosc x na ostatnia dozwolona */
pom=min(n,xs-x+1);
MEMSET( addr,CUR_COLOR,pom);
n-=pom;xs++;ys=y;
if(n>0) goto JESZCZERAZ;
return 1;
}

int picture::GetLine(pixpointer c_tar,pixco_t xx,pixco_t yy,pixco_t n) const
// Copy line from screen to bufor
{
pixpointer addr,tar=c_tar;
pixsize_t  x,y,xs,ys,pom;
if(  xx<0 || xx>MAXX || yy<0 || yy>MAXY || (xx+n)<0) /* Sprawdzanie */
	return 0;
if( n<=0  )
	return 0;
if( xx+n>MAXX )				  /* Obcinanie za MAXX */
	n=MAXX-xx+1;

xs=x=(pixsize_t) xx;
ys=y=(pixsize_t) yy;
JESZCZERAZ:
addr=CalcPixelAddr(xs,ys); /* Zmienia wartosc x na ostatnia dozwolona */
pom=min(n,xs-x+1);
MEMCPY( tar,addr,pom);
n-=pom;tar+=pom;xs++;ys=y;
if(n>0) goto JESZCZERAZ;
return 1;
}

int picture::PutLine(pixco_t xx,pixco_t yy,pixco_t n,const pixpointer c_src)
{
pixpointer addr,src=c_src;
pixsize_t  x,y,xs,ys,pom;
if(  xx>MAXX || yy<0 || yy>MAXY || (xx+n)<0) /* Sprawdzanie */
	return 0;
if( xx < 0 )                               /* Obcinanie przed 0 */
	{ xx=abs(xx);
	  n-=xx;
	  src+=xx;
	  xx=0;}
if( n<=0  )
	return 0;
if( xx+n>MAXX )				  /* Obcinanie za MAXX */
	n=MAXX-xx+1;

xs=x=(pixsize_t) xx;
ys=y=(pixsize_t) yy;
JESZCZERAZ:
addr=CalcPixelAddr(xs,ys); /* Zmienia wartosc x na ostatnia dozwolona */
pom=min(n,xs-x+1);
colorlinecopy( addr,src,pom);
n-=pom;src+=pom;xs++;ys=y;
if(n>0) goto JESZCZERAZ;
return 1;
}

int picture::PutPicture(pixco_t x1 ,pixco_t y1, const mem_picture& source) // Copy another picture at x,y
{
pixco_t yy1,xx1,i,len,n;
pixsize_t xp,yp;// zmienne tymczasowena parametry funkcji PixelAddres
if(x1>MAXX || y1>MAXY )
		return 0; /* Za ekranem */
xx1=x1;
len=source.GetLineLen();
n=source.GetMaxY();
if(x1+len<0 || y1+n<0 )
		return 0; /* Przed ekranem */
for(i=0;i<=n;i++)
	{
	yy1=y1+i;
	if(yy1>=0 && yy1<=MAXY)
		{
		xp=0,yp=i;// Zmieniane w wywolaniu
		PutLine( xx1,yy1,len,source.CalcPixelAddr(xp,yp));
		}
	}
return 1;
}

int picture::FlowPicture(const mem_picture& what )         // Copy another, same LINELEN picture at 0,0
	{if(LINELEN!=what.GetLineLen()) return 0;
	 return PutPicture(0,0,what);}

void picture::AllDefault()
{
target=NULL;   // Target picture or NULL
linebuf=NULL;
bufer=NULL;
curfont=&DefaultFont;
palet=&DefaultPalette;
CUR_COLOR=0; // Free color and set to 0 - transparent
BACKGROUND=16;// Default Black
CUR_X=CUR_Y=0;
CUR_MODE=TRANSPARENTPUT;
colorlinecopy=translinecopy;
StartX=StartY=0;
LINELEN=MAXX=MAXY=0;
XRATIO=YRATIO=1;
}

int picture::Fill(color_handle c)
{
/* Nieoptymalnie ale zawsze prawidlowo */
color_handle oldcolor=CUR_COLOR;
CUR_COLOR=c;
FillRectAngle(0,0,MAXX,MAXY);
CUR_COLOR=oldcolor;
return 1;
}

int picture::Clear()
{
return Fill(BACKGROUND);
}

int picture::ShiftLine(pixco_t x,pixco_t y,pixco_t shval)
// Shift line     - not tested
{
if(y<0 || y>MAXY || x>MAXX || x+shval>MAXX ) // Bledne dane
	return 0;
if(shval==0) return 1;
if(x<0)	x=0;
size_t linelen=LINELEN-x-abs(shval);
pixpointer buf=GetLineBuf();
if(shval>0)
	{
	GetLine(buf,x,y,linelen);
	PutLine( x+shval,y,linelen,buf);
	}
	else
	{
	GetLine(buf,x+abs(shval),y,linelen);
	PutLine( x,y,linelen,buf);
	}
return 1;
}

int picture::ShiftUp(pixco_t s)
{
int i;
if(s<=0) return 0;
pixpointer p=GetLineBuf();
for(i=s;i<GetMaxY();i++)
	{
	GetLine(p,0,i  ,LINELEN);
	PutLine(  0,i-s,LINELEN,p);
	}
return 0;

}

int picture::ShiftDown(pixco_t s)
{
int i;
if(s<=0) return 0;
pixpointer p=GetLineBuf();
for(i=GetMaxY()-s;i>s;i--)
	{
	GetLine(p,0,(pixco_t)i  ,LINELEN);
	PutLine(  0,(pixco_t)(i+s),LINELEN,p);
	}
return 0;
}


int picture::ShiftLeft(pixco_t s)
{
int i;
if(s<=0) return 0;
if(s>=MAXX) return Clear();
for(i=0;i<=MAXY;i++)
	ShiftLine(0,i,-s);
return 1;
}

int picture::ShiftRight(pixco_t s)
{
int i;
if(s<=0) return 0;
if(s>=MAXX) return Clear();
for(i=0;i<=MAXY;i++)
	ShiftLine(0,i,s);
return 1;
}


int picture::FillRectAngle(long x1,long y1,long x2,long y2)
{
unsigned int xx1,xx2, i ;
unsigned int yy1,yy2;

if( x2<x1 || y2<y1 || x2<0L || y2<0L || x1>MAXX || y1>MAXY ) return 0;
xx1 = ( x1>  0L ? x1:   0 );
yy1 = ( y1>  0L ? y1:   0 );
yy2 = ( y2<MAXY ? y2: MAXY );
xx2 = ( x2<MAXX ? x2: MAXX );
if((xx2=xx2-xx1+1)<=0) return 0;
while(yy1<=yy2)
	{
	FillLine((pixco_t)xx1,(pixco_t)yy1,(pixco_t)xx2);
	yy1++;
	}
return 1;
}

int picture::DrawLine( long int xp , long int yp ,long int xk , long int yk )
/* narysowanie lini w dowolnym kierunku algorytmen Bresenhama */
{
register int xxp;/* zastepniki parametrow */
register int yyp;
int	 xxk,yyk;
int dx,dy,x,y,p,c1,c2;/* zmienne pomocnicze */

if(xp<-MAXINT) xxp=-MAXINT;
   else if(xp>MAXINT) xxp=MAXINT;
	    else   xxp=(pixco_t) xp;
if(xk<-MAXINT) xxk=-MAXINT;
   else if(xk>MAXINT) xxk=MAXINT;
	    else   xxk=(pixco_t)xk;
if(yp<-MAXINT) yyp=-MAXINT;
   else if(yp>MAXINT) yyp=MAXINT;
	    else   yyp=(pixco_t)yp;
if(yk<-MAXINT) yyk=-MAXINT;
   else if(yk>MAXINT) yyk=MAXINT;
	    else   yyk=(pixco_t)yk;

dx=abs(xxk-xxp); dy=abs(yyk-yyp);

if(xxp>=0 && xxp<=MAXX && yyp>=0 && yyp<=MAXY )
	PlotPixel(xxp , yyp);
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
			PlotPixel(xxp , yyp);
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
			PlotPixel(xxp , yyp);
		}
	}
return 1;
}

int picture::FillEllipse(long int xxs,long int yys,long int bb,long int aa)
{
register int xxp,yyp;
register int xs=xxs;
register int ys=yys;
register int yi=0;
register int xi=0;
register int n=0;

long int a2=aa*aa;
long int b2=bb*bb;
long int a2s=a2;
long int b2s=b2*(2*aa+1);
long int DELTAi=2*(1-bb);

if(aa==0 || bb==0)
		return 0;
color_handle COL_CORE=GetColor();
RGB rgblocal=palet->Values(COL_CORE);
rgblocal.R/=2;rgblocal.G/=2;rgblocal.B/=2;
color_handle COL_BORDER=palet->Alloc(rgblocal,1);

if(aa<-MAXINT) yi=-MAXINT;
   else if(aa>MAXINT) yi=MAXINT;
	    else   yi=(pixco_t) aa;
yyp=MAXINT;
while( yi>=0 )
{
xxp=xs-xi;
if(yyp!=yi) // Nowa linia
	{
	n=2*xi+1;
	yyp= ys-yi;
	FillLine(xxp,yyp,n);
	yyp= ys+yi;
	FillLine(xxp,yyp,n);
	}
	else // Uzupelnienie lini o punkty brzezne
	{
	SetColor(COL_BORDER);
	xxp=xs+xi;  yyp= ys+yi;
	if(xxp>=0 && xxp<=MAXX && yyp>=0 && yyp<=MAXY )
		PlotPixel(xxp , yyp);
	xxp=xs-xi;  yyp= ys-yi;
	if(xxp>=0 && xxp<=MAXX && yyp>=0 && yyp<=MAXY )
		PlotPixel(xxp , yyp);
	xxp=xs+xi;  yyp= ys-yi;
	if(xxp>=0 && xxp<=MAXX && yyp>=0 && yyp<=MAXY )
		PlotPixel(xxp , yyp);
	xxp=xs-xi;  yyp= ys+yi;
	if(xxp>=0 && xxp<=MAXX && yyp>=0 && yyp<=MAXY )
		PlotPixel(xxp , yyp);
	SetColor(COL_CORE);
	}
yyp=yi; // zapaminetaj do porownania
BEZRYSOWANIA:
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

_1300:			/* krok poziomy */
if((DELTAi+DELTAi+b2s)>0L) goto _1240;
xi++;
a2s+=a2+a2;
DELTAi+=a2s;
continue;
//goto BEZRYSOWANIA;

_1380:			/* krok pionowy */
yi--;
b2s-=(b2+b2);
DELTAi-=b2s;
}

return 1;
}

int picture::DrawEllipse(long int xxs,long int yys,long int bb,long int aa)
{
register int xxp,yyp;
register int xs=xxs;
register int ys=yys;
register int yi=0;
register int xi=0;

long int a2=aa*aa;
long int b2=bb*bb;
long int a2s=a2;
long int b2s=b2*(2*aa+1);
long int DELTAi=2*(1-bb);

if(aa==0 || bb==0) return 0;

if(aa<-MAXINT) yi=-MAXINT;
   else if(aa>MAXINT) yi=MAXINT;
	    else   yi=(pixco_t) aa;

while( yi>=0 )
{
xxp=xs+xi;  yyp= ys+yi;
if(xxp>=0 && xxp<=MAXX && yyp>=0 && yyp<=MAXY )
	PlotPixel(xxp , yyp);
xxp=xs-xi;  yyp= ys-yi;
if(xxp>=0 && xxp<=MAXX && yyp>=0 && yyp<=MAXY )
	PlotPixel(xxp , yyp);
xxp=xs+xi;  yyp= ys-yi;
if(xxp>=0 && xxp<=MAXX && yyp>=0 && yyp<=MAXY )
	PlotPixel(xxp , yyp);
xxp=xs-xi;  yyp= ys+yi;
if(xxp>=0 && xxp<=MAXX && yyp>=0 && yyp<=MAXY )
	PlotPixel(xxp , yyp);


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

return 1;
}

/*
int picture::FillEllipse(long xxs,long yys,long  aa,long  bb)
{
if(bb<=1){
	PlotPixel(xxs,yys);
	return 1;}
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
		FillLine(xa,ya,x);

	ya=ys-y;
	if(ya>=0 && ya<=MAXY)
		FillLine(xa,ya,x);
	}
return 1;
}
*/


extern font8bit DefaultFont;            //Font standartowy
extern palette DefaultPalette;          //Standartowa paleta 256 kolorow
extern void error(char*,int);           //Ogolna funkcja obslugi bledu

picture* screen::StartMenu()
{
picture* pom=NULL;
int i,mm,ColorScale;
/* Wybor trybu pracy ekranu */
for(i=1;i<NumberOfTypes;i++)
	{
	printf("%3d) %s\n",i,ScreenTypes[i].names);
	}
USER_ERROR:
{
char bufor[100];
printf("Device ? :");
gets(bufor);
sscanf(bufor,"%d",&mm);
printf("Color Cube (3..6) or Gray Scale (-1..-255)? :");
gets(bufor);
sscanf(bufor,"%d",&ColorScale);
if(mm>0 && mm<NumberOfTypes)
	pom=ScreenTypes[mm].make();
	else
        goto USER_ERROR;
}
/* Sprawdzenie czy sie udalo */
if(pom==NULL)
	{
	error("Graphix initialization error !!!",1);
	}

// Operacje ustalajace parametry
pom->SetFont(*font::FontByNumber(0));
pom->SetPalette(DefaultPalette);
DefaultPalette.fromhardware(); // Init Values for this screen in RGB table
if(ColorScale>0)
                DefaultPalette.cube(ColorScale);
                else
                DefaultPalette.grayscale((unsigned char)abs(ColorScale));
DefaultPalette.tohardware();
return pom;
}
