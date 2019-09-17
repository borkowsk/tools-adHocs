/*
test dla niskopoziomowego interfacu do grafiki
*/
#include <conio.h>
#include <time.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include "BiosGraf.h"
#include "TopVga.h"
#include "BIOSCHAR.hpp"
#include "pipek.hpp"

int _M_X=0;
int _M_Y=0;

int MouseTo13h() // dopasowuje przestrzen myszki 640/200 do 320 na 200
{
int pom=Interface::CheckClik();
_M_X=Interface::GetMosX()/2;
_M_Y=Interface::GetMosY();
return pom;
}

inline int MouseX() {return _M_X;}
inline int MouseY() {return _M_Y;}

unsigned char far virtualscreen[200][320];/* Virtualny ekran trybu 320x200 */
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


#define screen (virtualscreen)
struct {int x;int y;int c;} gw[]=
{
{15,15,64},{100,100,127},{200,150,254},{300,100,63},{250,127,127},
{25,18,125},{140,95,32},{210,115,254},{288,110,254},{150,127,48},
{50,28,127},{124,152,32},{201,125,127},{188,121,254},{105,157,48}
};

int Ilosc_gwiazd=sizeof(gw)/(3*sizeof(int));
unsigned int Numizumi=255;

void PutGw(int x,int y,int c)
{
screen[y][x]=c;
c--;
screen[y+1][x]=c;
screen[y-1][x]=c;
screen[y][x+1]=c;
screen[y][x-1]=c;
c--;
screen[y+1][x+1]=c;
screen[y-1][x-1]=c;
screen[y-1][x+1]=c;
screen[y+1][x-1]=c;
screen[y+2][x]=c;
screen[y-2][x]=c;
screen[y][x+2]=c;
screen[y][x-2]=c;
c--;
screen[y+2][x+2]=c;
screen[y-2][x-2]=c;
screen[y-2][x+2]=c;
screen[y+2][x-2]=c;
screen[y+3][x]=c;
screen[y-3][x]=c;
screen[y][x+3]=c;
screen[y][x-3]=c;
}

void PutE()
{
static int color=0;
static int licznik=0;
for(int i=180;i<=199;i++)
   _fmemset(screen[i],color+64+(i-180),320);
licznik++;
if(licznik==5){
	color=(color+1)%8;
	licznik=0;}
_fmemset(screen[199],127,Numizumi);
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
     if(X<0 || X>319 || Y<0 || Y>199 ) continue;
     screen[Y][X]= mapa[j][i];
     }
}

void PutCursor()
{
int x=MouseX();
int y=MouseY();
screen[y][x]=255;
if(y<188 && y>2 )
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
#define screen(_Y_,_X_) *((unsigned char far*)MK_FP(0xA000,0x0)+(320*_Y_+_X_))
int r1=e/10;
int r2=e/20;
for(int i=n;i>0;i--)
	{
	sound((pixels[i].c=e-random(r1))/3);
	pixels[i].rx=random(r1)-r2;
	pixels[i].ry=random(r1)-r2;
	pixels[i].x=x+pixels[i].rx;
	pixels[i].y=y+pixels[i].ry;
	if(pixels[i].x<0 || pixels[i].x>319) continue;
	if(pixels[i].y<0 || pixels[i].y>199) continue;
	screen(pixels[i].y,pixels[i].x)=pixels[i].c;
	}
nosound();
for(i=0;i<e/4;i++)
	{
    for(int j=0;j<n;j++)
	{
	x=pixels[j].x;
	if(x<0 || x>319) continue;
	y=pixels[j].y;
	if(y<0 || y>199) continue;
	screen(y,x)=pixels[j].oldc;/* wygaszanie */

	x=pixels[j].x+=pixels[j].rx;
	if(x<0 || x>319) continue;
	y=pixels[j].y+=pixels[j].ry;
	if(y<0 || y>199) continue;
	pixels[j].oldc=screen(pixels[j].y,pixels[j].x);

	pixels[j].c--;
	screen(y,x)=pixels[j].c;

	if(i%30) pixels[j].ry++;/* grawitacja */
	}
	}
nosound();
}

long starttime;
long endtime;

void SmartDelay(long int i)
{
static long dela=100;
long endtime=clock();

double sec=(endtime-starttime)/(float)CLK_TCK;
if(sec==0) return;
sec=i/sec; // ile klatek na sekunde
if(sec<=16) dela=0; // za malo
   else
   if( sec<=25 ) dela-=5;// zmiejsza, zeby przyspieszyc
     else        dela+=20;// zwieksza zeby zwolnic
if(dela>0)
	delay(dela);
}

void GameOver(int x, int y,long int i)
{
long endtime;
double sec;
COLOR=253;
Line13H(x+8,y+8,160,199);
PutUfo(x,y);
wait_for_frame();
_fmemcpy(MK_FP(0xA000,0x0000),virtualscreen,320*200);
Bum(160,199,500,254);
endtime=clock();
BiosGotoXY(12,10);
printf("G A M E  O V E R");
BiosGotoXY(13,12);
sec=(endtime-starttime)/(float)CLK_TCK;
printf("TIME %6.2f s\n",sec);
//printf("Szybkosc: %6.2f pic/sec\n",i/sec);
getch();
}

main()
{
int x=160,y=100,rx=1,ry=0,color=31,xs,ys;
long int i=0;
randomize();
if(!BiosScreen(0x13))
	{
	fprintf(stderr,"VGA required!\n");
	return 1;
	}
Interface::MousePresent();
Interface::UnVisCursor();
ColorContinum(BR_G);
SCREEN=(unsigned char far*)virtualscreen;
xs=MAXX/2;
ys=MAXY/2;
starttime=clock();
do{
i++;// licznik
_fmemset(virtualscreen,0,320*200);
x+=rx;
y+=ry;
if(x<0 || x>320) rx=-rx;
if(y<0 || y>200) ry=-ry;
if(random(50)==0)
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
for(int j=0;j<Ilosc_gwiazd;j++)
	{
	PutGw(gw[j].x,gw[j].y,gw[j].c);
	}
FillCircle13H(xs,ys,65);
PutE();

if( MouseTo13h() && Numizumi>0)
	{
	COLOR=254;
	Line13H(160,199,MouseX(),MouseY());
	if(x<MouseX() && MouseX()<x+16 &&
	   y<MouseY() && MouseY()<y+16 )
	   {
	   PutUfo(x,y);
	   _fmemcpy(mapa,mapa2,sizeof(mapa));
	   wait_for_frame();
	   _fmemcpy(MK_FP(0xA000,0x0000),virtualscreen,320*200);
	   Bum(MouseX(),MouseY(),500,254);
	   Numizumi--;
	   x=random(320);
	   y=random(100);
	   rx=random(8)-random(16);
	   ry=random(8)-random(16);
	   }
	   else
	   {
	   PutUfo(x,y);
	   PutCursor();
	   wait_for_frame();
	   _fmemcpy(MK_FP(0xA000,0x0000),virtualscreen,320*200);
	   Bum(MouseX(),MouseY(),64,64);
	   Numizumi--;
	   }
	}
    else
    {
    if(y>150)
       if(x>150 && x<170) break;/* Trafiony */
     PutUfo(x,y);
     PutCursor();
     _fmemcpy(MK_FP(0xA000,0x0000),virtualscreen,320*200);
     }
SmartDelay(i);
}while(!kbhit());

GameOver(x,y,i);
BiosScreen(0x3);
*(SCREEN+2)='B';
*(SCREEN+4)='Y';
*(SCREEN+6)='E';
*(SCREEN+8)='!';
return 0;
}