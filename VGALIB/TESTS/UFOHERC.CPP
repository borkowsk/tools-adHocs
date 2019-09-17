#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include "BiosGraf.h"
#include "TopVga.h"
#include "BIOSCHAR.hpp"
#include "pipek.hpp"

void line2herc(int y,unsigned char far* sou);
int hercgraph();

unsigned char far virtualscreen[175][360];/* Virtualny ekran trybu 13H */



unsigned char mapa[16][16]=
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,54,54,54,54,0,0,0,0,0,0,
0,0,0,0,0,55,55,55,55,55,55,0,0,0,0,0,
0,0,0,0,56,56,215,56,56,215,56,56,0,0,0,0,
0,0,0,0,56,56,56,58,58,56,56,56,0,0,0,0,
0,0,0,0,56,56,56,58,58,56,56,56,0,0,0,0,
0,0,56,56,56,56,56,56,56,56,56,56,56,56,0,0,
0,56,56,56,56,56,56,56,56,56,56,56,56,56,56,0,
57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,
0,56,56,56,56,56,56,56,56,56,56,56,56,56,56,0,
0,0,0,56,56,56,56,56,56,56,56,56,56,0,0,0,
0,0,0,0,56,56,56,56,56,56,56,56,0,0,0,0,
0,0,0,0,0,55,55,55,55,55,55,0,0,0,0,0,
0,0,0,0,0,0,54,54,54,54,0,0,0,0,0,0,
0,0,0,0,0,0,0,53,53,0,0,0,0,0,0,0,
0,0,0,0,0,0,9,0,0,9,0,0,0,0,0,0
};

unsigned char mapa2[16][16]=
{
69,9,0,0,0,0,0,0,0,0,0,0,0,0,9,69,
9,69,67,0,0,0,0,0,0,0,0,0,0,67,69,9,
0,67,69,0,0,0,0,0,0,0,0,0,9,69,67,0,
0,0,9,69,9,0,0,0,0,0,0,9,69,9,0,0,
0,0,0,9,69,9,0,0,0,0,0,69,9,0,0,0,
0,0,0,0,9,69,9,6,6,9,69,9,0,0,0,0,
0,0,0,0,0,9,69,67,67,69,9,0,0,0,0,0,
0,0,0,0,0,9,67,69,69,67,9,0,0,0,0,0,
0,0,0,0,0,9,67,69,69,67,9,0,0,0,0,0,
0,0,0,0,0,9,69,9,9,69,9,0,0,0,0,0,
0,0,0,0,0,69,9,0,0,9,69,9,0,0,0,0,
0,0,0,0,69,9,0,0,0,0,9,69,9,0,0,0,
0,0,9,69,9,0,0,0,0,0,0,9,69,9,0,0,
9,67,69,9,0,0,0,0,0,0,0,0,9,69,67,9,
9,69,67,9,0,0,0,0,0,0,0,0,0,67,69,9,
69,9,9,0,0,0,0,0,0,0,0,0,0,0,9,69
};


#define screen virtualscreen

void putvirtual()
{
for(int i=0;i<175;i++)
   line2herc(i,virtualscreen[i]);
}

void PutE()
{
for(int i=165;i<175;i++)
   _fmemset(virtualscreen[i],64+(i-180),360);
}

void PutUfo(int x,int y)
{
#define screen virtualscreen
int X,Y;
for(int i=0;i<16;i++)
  for(int j=0;j<16;j++)
     {
     if(mapa[j][i]==0) continue;
     Y=y+j;
     X=x+i;
     if(X<0 || X>359 || Y<0 || Y>174 ) continue;
     screen[Y][X]= mapa[j][i];
     }
}

void PutCursor()
{
int x=Interface::GetMosX();
int y=Interface::GetMosY();
if(x<0) x=0;
if(x>MAXX) x=MAXX;
if(y<0) y=0;
if(y>MAXY) y=MAXY;
screen[y][x]=255;
if(y<172 && y>2 )
	{
	screen[y+1][x]=255;
	screen[y-1][x]=255;
	screen[y][x+1]=255;
	screen[y][x-1]=255;
	screen[y+2][x]=255;
	screen[y-2][x]=255;
	screen[y][x+2]=255;
	screen[y][x-2]=255;
	}
}

void Bum(int x,int y,int n, int e)
{
static struct {int x;int y;int rx;int ry;unsigned char c;unsigned char oldc;} pixels[512];
COLOR=e;
circle(x,y,e/4);
COLOR=e-1;
circle(x,y,e/8);
}

void GameOver(int x, int y)
{
COLOR=253;
Line13H(x+8,y+8,180,174);
PutUfo(x,y);
Bum(180,174,500,254);
putvirtual();
delay(500);
BiosScreen(0x7);
gotoxy(12,10);
printf("G A M E  O V E R");
getch();
}

main()
{
extern unsigned long int LINELEN;
int x=160,y=100,rx=1,ry=0,color=31,xs,ys,i=0;
randomize();
hercgraph();

Interface::MousePresent();
Interface::UnVisCursor();
SCREEN=(unsigned char far*)virtualscreen;
MAXX=359;
MAXY=174;
LINELEN=360;
XRATIO=750;
YRATIO=1000;
xs=MAXX/2;
ys=MAXY/2;

do{
i++;
_fmemset(virtualscreen,0,360*175);
x+=rx;
y+=ry;
if(x<0 || x>MAXX) rx=-rx;
if(y<0 || y>MAXY) ry=-ry;
if(random(100)==0)
	{
	if(abs(rx=rx+random(8)-random(16))>20)
		      rx=1;
	if(abs(ry=ry+random(8)-random(16))>20)
		      ry=1;
	}
if(i%50==0)
	{
	xs++;
	ys++;
	if(i%1000==0) color--;
	}
COLOR=color;
FillCircle13H(xs,ys,50);
PutE();

if(Interface::CheckClik())
	{
	COLOR=254;
	Line13H(180,174,Interface::GetMosX(),Interface::GetMosY());
	if(x<Interface::GetMosX() && Interface::GetMosX()<x+16 &&
	   y<Interface::GetMosY() && Interface::GetMosY()<y+16 )
	   {
	   PutUfo(x,y);
	   _fmemcpy(mapa,mapa2,sizeof(mapa));
	   Bum(Interface::GetMosX(),Interface::GetMosY(),500,254);
	   x=random(360);
	   y=random(80);
	   rx=random(8)-random(16);
	   ry=random(8)-random(16);
	   putvirtual();
	   }
	   else
	   {
	   PutUfo(x,y);
	   PutCursor();
	   Bum(Interface::GetMosX(),Interface::GetMosY(),64,64);
	   putvirtual();
	   }
	}
    else
    {
    if(y>145)
       if(x>170 && x<190) break;/* Trafiony */
     PutUfo(x,y);
     PutCursor();
     putvirtual();
     }
}while(!kbhit());
GameOver(x,y);

*(SCREEN+2)='B';
*(SCREEN+4)='Y';
*(SCREEN+6)='E';
*(SCREEN+8)='!';
}