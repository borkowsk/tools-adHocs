#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <math.h>
#include <graph2.h>

typedef unsigned char byte;
typedef unsigned int word;
typedef byte wiersz320[320];

wiersz320 far *ekranVGA=NULL;
void      far *ekranBYTE=NULL;

inline int inscr(long int x, long int y)
{
return (x>=0 && x<=MaxX && y>=0 && y<=MaxY);
}

class mat_point
	{
protected:
	float xf,yf,zf;
	float mass;
	long int xscr,yscr,zscr;
	int color;
public:
	mat_point()
		{ xf=xf=zf=mass=0.0;color=DARKGRAY; }
	mat_point(double lx,double ly,double lz,double lm,int lc)
		{ xf=lx;yf=ly;zf=lz;mass=lm;color=lc ;}
	~mat_point()
		{ sound(100);delay(mass/10);nosound();color=RED;}
	void set(double lx,double ly,double lz,double lm,int lc)
		{ xf=lx;yf=ly;zf=lz;mass=lm;color=lc ;}
	void put()
		{
		xscr=xf;
		yscr=yf;
		zscr=zf;
		if( inscr(xscr,yscr) )
		   putpixel(xscr,yscr,color);
		}
	void put13H()
		{
		xscr=xf;
		yscr=yf;
		zscr=zf;
		if( inscr(xscr,yscr) )
			     ekranVGA[yscr][xscr]=color;
		}
	};

class gwiazda : mat_point
	{
	float rkuli;
public:
	gwiazda(double lx,double ly,double lz,double lm,int lc,double lr): mat_point(lx,ly,lz,lm,lc)
		{ rkuli=lr;}
	~gwiazda()
		{sound(200);delay(mass/10);nosound();color=LIGHTRED;}
	void put();
	void put13H();
	};

void gwiazda::put()
{int y,x,xa,ya,xe,col,verybig=0;
unsigned long int y2,a2,b2,a,b;
/* obliczenie wspolzednych iwymiarow obrazu */
	/* prowizoryczne */
xscr=xf;
yscr=yf;
zscr=zf;
a=b=rkuli;
/*******************************************/

b*=0.80 ;// poprawka na nierownomiernosc ekranu
verybig=(a>286);

if(inscr(xscr,yscr)||
  inscr(xscr+a,yscr+b)||inscr(xscr+a,yscr-b)||
  inscr(xscr-a,yscr+b)||inscr(xscr-a,yscr-b))
  {/* warunek spelniony = jest na ekranie */
  b2=(long int)b*b;
  a2=(long int)a*a;
  for(y=0;y<b;y++)
	{
	y2=(long int)y*y;

	if(verybig)  x=sqrt(((double)a2*(double)a2-(double)y2*(double)a2)/(double)b2);
		else x=sqrt((a2*b2-y2*a2)/b2);

	col=random(color)+1;
	xe=xscr+x;
	if(xe>MaxX) xe=MaxX;
	xa=xscr-x;
	if(xa<0) xa=0;
	for(;xa<xe;xa++)
		{
		ya=yscr+y;
		if(ya>=0 && ya<=MaxY)
			ekranVGA[ya][xa]=col;
			/*putpixel(xa,ya,col);*/
		ya=yscr-y;
		if(ya>=0 && ya<=MaxY)
			ekranVGA[ya][xa]=col;
			/*putpixel(xa,ya,col);*/
		}
	}
   }
/* koniec funkcji */}


void kolo(int xs,int ys,int b)
{int y,x,xa,ya,xe,a,color;
long int y2,a2,b2;
a=b;
b*=0.80 ;
b2=(long int)b*b;
a2=(long int)a*a;
for(y=0;y<b;y++)
	{
	y2=(long int)y*y;
	x=sqrt((a2*b2-y2*a2)/b2);

	color=random(getmaxcolor())+1;
	xe=xs+x;
	if(xe>MaxX) xe=MaxX;
	xa=xs-x;
	if(xa<0) xa=0;
	for(;xa<xe;xa++)
		{
		/*putpixel(xa,ys+y,color);
		putpixel(xa,ys-y,color);*/
		ya=ys+y;
		if(ya>=0 && ya<=MaxY)
			ekranVGA[ya][xa]=color;
		ya=ys-y;
		if(ya>=0 && ya<=MaxY)
			ekranVGA[ys-y][xa]=color;
		}
	}
}

int main(void)
{
int i,r;
gwiazda pierwsza(50.,100.,10.,10.,100,50.),druga(200.,100.,50.,50.,15,100. );
Initialize();
/*DrawBorder();*/
/*ekranBYTE=MK_FP( 0xB000 ,0x0000 ); ekran tekstowy herculesa*/
/*ekranBYTE=MK_FP(0xB800 ,0x0000);  ekran tekstowy VGA */
ekranBYTE=MK_FP(0xA000,0x0000);
ekranVGA=(wiersz320 *)ekranBYTE;
ekranVGA[0][0]=15;
putpixel(0,1,24);
randomize();
   for(i=1;i>0;i++)
		{
		kolo( 50,50 ,50 );
	   pierwsza.put();
	   druga.put();
	   if(kbhit())
		exit(0);
		}
   /* clean up */
   getch();
   closegraph();
   return 0;
}
