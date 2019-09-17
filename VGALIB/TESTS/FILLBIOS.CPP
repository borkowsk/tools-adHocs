#include <stdio.h>
#include <conio.h>
#include <dos.h>

#include <ctype.h>
#include <stdlib.h>
#include "BIOSGRAF.h"

#include <math.h>
#include <values.h>
#include <setjmp.h>
#include <float.h>
#include <signal.h>

jmp_buf jumper,brjumper;
void Catcher(int sig)
{
   printf("Caught exception !\n");
   _clear87();
   _fpreset();
   longjmp(jumper,1);
}

int BrCatcher(void)
{
   printf("User Break !\n");
   _clear87();
   _fpreset();
   longjmp(brjumper,1);
   return 0;
}

#define BEEP '\006'
/* stale dokladnosci liczenia */
#define MAX_VAL 3.4e48
#define MIN_VAL -3.4e48
#define _2Pi    (3.1415926535897932385*2)
#define LINELEN 320
#define OFFSET13H( _X_ , _Y_ )  ((LINELEN*(_Y_))+(_X_))

double jedenx=0.0, jedeny=0.0;/* inicjacja jest zmieniana w init ! */
int my=0,mx=0,mc=0,N=8;
double X=10.0,Y=8.0;
unsigned int mod=0x3;

void FuncScale()
{
int i,j;

for( j=0 ; j < 256 ; j++ )
	{
	for( i=MAXX-MAXX/20 ; i < MAXX ; i++ )
		BiosSetPixel( i , j, j );
	if(kbhit())break;
	}
}

void randomfill()
{
int x,y;
int vx=random(10);
int vy=random(10);
BiosScreen(mod);
ColorContinum(2);
FuncScale();
x=random(200);
y=random(200);
do{
x+=vx;
y+=vy;
vx=random(10)-random(10);
vy=random(10)-random(10);
if(x>=0&&x<200&&y>=0&&y<200)
	{
	int ox=x,oy=y;
	SCREEN[OFFSET13H(ox,oy)]++;
	}
	else
	{
	int ox=x,oy=y;
	if(oy<0){vy=abs(vy); }
	   else
	   if(oy>=200){vy=-abs(vy); }/* zmiana kierunku */
	if(ox<0){vx=abs(vx); }
	   else
	   if(ox>=200){vx=-abs(vx); }
	}
}while(!kbhit());
}

void exper()
{
double x,y;
double vx=(random(RAND_MAX)*2.)/RAND_MAX;
double vy=(random(RAND_MAX)*2.)/RAND_MAX;
double a=(random(RAND_MAX)*5.)/RAND_MAX;
BiosScreen(mod);
ColorContinum(2);
FuncScale();
x=random(200);
y=random(200);
do{
x+=vx*cos(a*(y/200.0)*_2Pi);
y+=vy*sin(a*(x/200.0)*_2Pi);
if(x>=0&&x<200&&y>=0&&y<200)
	{
	int ox=x,oy=y;
	SCREEN[OFFSET13H(ox,oy)]++;
	}
	else
	{
	if(y<0){vy=fabs(vy); }
	   else
	   if(y>=200){vy=-fabs(vy); }/* zmiana kierunku */
	if(x<0){vx=fabs(vx); }
	   else
	   if(x>=200){vx=-fabs(vx); }
	}
}while(!kbhit());
}

void exper1()
{
double x,y;
double vx=(random(RAND_MAX)*2.)/RAND_MAX;
double vy=(random(RAND_MAX)*2.)/RAND_MAX;
BiosScreen(mod);
ColorContinum(2);
FuncScale();
x=random(200);
y=random(200);
do{
x=fabs(cos((y/200.0)*_2Pi)+vx)*200;
y=fabs(sin((x/200.0)*_2Pi)+vy)*200;
if(x>=0&&x<200&&y>=0&&y<200)
	{
	int ox=x,oy=y;
	SCREEN[OFFSET13H(ox,oy)]++;
	}
	else
	{
	if(y<0){vy=fabs(vy); }
	   else
	   if(y>=200){vy=-fabs(vy); }/* zmiana kierunku */
	if(x<0){vx=fabs(vx); }
	   else
	   if(x>=200){vx=-fabs(vx); }
	}
}while(!kbhit());
}

void exper2()
{
double x,y;
double vx=(random(RAND_MAX)*2.)/RAND_MAX;
double vy=(random(RAND_MAX)*2.)/RAND_MAX;
BiosScreen(mod);
ColorContinum(2);
FuncScale();
x=random(200);
y=random(200);
do{
x+=vx+cos((y/200.0)*_2Pi);
y+=vy+sin((x/200.0)*_2Pi);
if(x>=0&&x<200&&y>=0&&y<200)
	{
	int ox=x,oy=y;
	SCREEN[OFFSET13H(ox,oy)]++;
	}
	else
	{
	if(y<0){vy=fabs(vy); }
	   else
	   if(y>=200){vy=-fabs(vy); }/* zmiana kierunku */
	if(x<0){vx=fabs(vx); }
	   else
	   if(x>=200){vx=-fabs(vx); }
	}
}while(!kbhit());
}

void exper3()
{
double x,y;
double vx=(random(RAND_MAX)*2.)/RAND_MAX;
double vy=(random(RAND_MAX)*2.)/RAND_MAX;
BiosScreen(mod);
ColorContinum(2);
FuncScale();
x=random(200);
y=random(200);
do{
x+=vx*cos(sinh((y/200.0)*_2Pi));
y+=vy*sin(cosh((x/200.0)*_2Pi));
if(x>=0&&x<200&&y>=0&&y<200)
	{
	int ox=x,oy=y;
	SCREEN[OFFSET13H(ox,oy)]++;
	}
	else
	{
	if(y<0){vy=fabs(vy); }
	   else
	   if(y>=200){vy=-fabs(vy); }/* zmiana kierunku */
	if(x<0){vx=fabs(vx); }
	   else
	   if(x>=200){vx=-fabs(vx); }
	}
}while(!kbhit());
}

void exper4()
{
double x,y;
double vx=(random(RAND_MAX)*2.)/RAND_MAX;
double vy=(random(RAND_MAX)*2.)/RAND_MAX;
BiosScreen(mod);
ColorContinum(2);
FuncScale();
x=random(200);
y=random(200);
do{
x+=vx*cos(cosh((y/200.0)*_2Pi));
y+=vy*sin(sinh((x/200.0)*_2Pi));
if(x>=0&&x<200&&y>=0&&y<200)
	{
	int ox=x,oy=y;
	SCREEN[OFFSET13H(ox,oy)]++;
	}
	else
	{
	if(y<0){vy=fabs(vy); }
	   else
	   if(y>=200){vy=-fabs(vy); }/* zmiana kierunku */
	if(x<0){vx=fabs(vx); }
	   else
	   if(x>=200){vx=-fabs(vx); }
	}
}while(!kbhit());
}

void horse()
{
int register x,y;
int vx=random(3)+1,vy=random(5)+1;
BiosScreen(mod);
ColorContinum(2);
FuncScale();
x=random(200);
y=random(200);
do{
x+=vx;
y+=vy;
if(x>=0&&x<200&&y>=0&&y<200)
	SCREEN[OFFSET13H(x,y)]++;
	else
	{
	int ox=x,oy=y;
	if(oy<0){vy=abs(vy); }
	   else
	   if(oy>=200){vy=-abs(vy); }/* zmiana kierunku */
	if(ox<0){vx=abs(vx); }
	   else
	   if(ox>=200){vx=-abs(vx); }
	}
}while(!kbhit());
}

void realvector()
{
double x,y;
double vx=(random(RAND_MAX)*3.)/RAND_MAX;
double vy=(random(RAND_MAX)*5.)/RAND_MAX;
BiosScreen(mod);
ColorContinum(3);
FuncScale();
x=(random(RAND_MAX)*200.)/RAND_MAX;
y=(random(RAND_MAX)*200.)/RAND_MAX;
do{
x+=vx;
y+=vy;
if(x>=0&&x<200&&y>=0&&y<200)
	{
	int ox=x,oy=y;
	SCREEN[OFFSET13H(ox,oy)]++;
	}
	else
	{
	if(y<0){vy=fabs(vy); }
	   else
	   if(y>=200){vy=-fabs(vy); }/* zmiana kierunku */
	if(x<0){vx=fabs(vx); }
	   else
	   if(x>=200){vx=-fabs(vx); }
	}
}while(!kbhit());
}

void symetric()
{
int register xi,yi;
static long licz=0;
double x,y,x2,y2,oldx,oldy;

#define a 1.890
#define b -1.100
#define c 0.170
#define d -0.790
#define x3 (x2*oldx)
#define y3 (y2*oldy)

BiosScreen(mod);
ColorContinum(2);
FuncScale();
x=0.5;
y=0.3;
X=10;
Y=10;
do{
oldx=x;
oldy=y;
x2=x*x;
y2=y*y;
x=a*oldx+b*oldx*(x2+y2);x+=c*(x3-3.0*oldx*y2);x+=d*(x2-y2);
y=a*oldy+b*oldy*(x2+y2);y+=c*(3.0*x2*oldy-y3);y-=2.0*d*oldx*oldy;
oldx=x*X;
oldy=y*Y;
if(oldx>=0 && oldx<MAXINT && oldy>=0 && oldy<MAXINT)
	{
	xi=oldx;
	yi=oldy;
	if(xi>=0&&xi<200&&yi>=0&&yi<200)
		SCREEN[OFFSET13H(xi,yi)]++;
	}
licz++;
}while(1);
}

void closing(void)
{
BiosScreen(0x3);
return ;
}

void init(void)
{

int value;
static char flag=0;
static twochar pom;
char tab[80],*end,command;
pom=BiosDevices();
if(pom.one>7)
	mod=0x13;
	else
	{
	printf("VGA reguaired !");
	exit(1);
	}
BiosScreen(mod);
do{
setjmp(brjumper);
value = setjmp(jumper);
if (value != 0)
   {
   printf("Press any key !\n");
   getch();
   signal(SIGFPE, Catcher);
   }
BiosScreen(0x3);
printf("\n               VGA - FILLING                              \n"
       "------------------------------------------------------------\n");
printf(" Q - exit\n");
printf(" 1 - random fill\n");
printf(" 2 - horse fill\n");
printf(" 3 - vector fill\n");
printf(" 4..- experiments\n");
printf(" \n");
printf(" \nPress 1..9 or Q !\n");

command=getch();

switch (toupper(command)) {
case 'Q':exit(0);
case '1':randomfill();break;
case '2':horse();break;
case '3':realvector();break;
case '4':exper();break;
case '5':exper1();break;
case '6':exper2();break;
case '7':exper3();break;
case '8':exper4();break;
default:putchar(BEEP);break;
}
clrscr();
}while(command!='`');
  return ;
/* end initialization */
}

int main(void)
{
randomize();
atexit(closing);
setjmp(jumper);
signal(SIGFPE, Catcher);
ctrlbrk(BrCatcher);
setjmp(brjumper);

do{
  init();
}while(1);

return 0;
}
