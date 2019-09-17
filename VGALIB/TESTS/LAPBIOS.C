#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "BIOSGRAF.h"

#include <math.h>
#include <values.h>
#include <setjmp.h>
#include <float.h>
#include <signal.h>

jmp_buf fpjumper,brjumper;

void FpCatcher(int sig)
{
   _clear87();
   _fpreset();
   signal(SIGFPE,FpCatcher);
   longjmp(fpjumper,1);
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

int NUMBER=150;
char outputname[100]="nul";/* nazwa zbioru z zapisem wynikow */
char pattern[100]="10";/* wzor strategi w funkcji lapunowa */
int np=sizeof(pattern)-1;/* i jego rozmiar */

double jedenx=0.0, jedeny=0.0;
float XS=0.0,YS=0.0,X=6.0,Y=5.0;/* inicjacja jest zmieniana w init ! */
int my=0,mx=0,mc=0,N=8;
FILE* trout;

void FuncScale()
{
int i,j;
FuncContinum();
if(NUMCOLOR>8 && NUMCOLOR<=16)
for( i=0 ; i < 256 ; i++ )
	{
	for( j=MAXY-MAXY/20 ; j < MAXY ; j++ )
		BiosSetPixel( i , j, i/16 );
	if(kbhit())break;
	}
else
for( i=0 ; i < 256 ; i++ )
	{
	for( j=MAXY-MAXY/20 ; j < MAXY ; j++ )
		BiosSetPixel( i , j, i );
	if(kbhit())break;
	}
}


double lapunBIN(double a, double b)
/* szacowanie funkcji lapunowa przy zmiennej strategi */
{
jmp_buf oldjmp;
oldjmp[0]=fpjumper[0];
double total=0.0,wx=0.0,aktr=0.0,aktrWx=0.0,logz2=0.0,ReturnV;
register int  i=0;
register char* p=pattern;

logz2=log10(2.0);
wx=0.0000000001;
if(a==0.0) return 1.0;
if(b==0.0) return 1.0;

/* Powrot przez skok do ENDE; */
if(setjmp(fpjumper)!=0)
	{ ReturnV=0.0;
	  goto ENDE;}

for(i=1;i<=NUMBER;i++)
	{
       if (total>MAX_VAL)
			break;
       if (total<MIN_VAL)
			break;

       if(p[i%np]=='0') aktr=a;
		   else aktr=b;

       /* x <- rx(1-x) funkcja logistyczna */

       aktrWx=aktr*wx;
       wx=aktrWx*(1-wx);

//       if (wx>MAX_VAL) break ;
//       if (wx<MIN_VAL) break ;

       aktr=fabs(aktr-2.0*aktrWx);

//       if (aktr>MAX_VAL) break ;
//       if (!(aktr>0)) break ;

       aktr=log10(aktr);

//       if (aktr>MAX_VAL) break ;
//       if (aktr<MIN_VAL) break ;

       aktr=aktr/logz2;

//       if (aktr>MAX_VAL) break ;
//       if (aktr<MIN_VAL) break ;

       total=total+aktr;

	}

if((i-=1)>0){
	     if((total/=i)>0) ReturnV=0.0;
			 else ReturnV=total;
	     }
	     else ReturnV=MAX_VAL;
ENDE:
fpjumper[0]=oldjmp[0];
return ReturnV;
}



void schem(void)
{
register int y=0;
register int x=0;
register int c=0;
double xr=0.0,yr=0.0;
FuncScale();
   for(y=1;y<my;y+=5)
    for(x=1;x<mx;x+=5)
	{
	 BiosSetPixel(x,y,x);
	 xr=(double)x/jedenx+XS;
	 yr=(double)y/jedeny+YS;
	 c=N*lapunBIN(xr,yr);
	 COLOR=-(c%mc);
	 BiosFillBar(x-2,y-2,x+2,y+2);
	 if(kbhit() && getch()==27 ) return;
	}
FuncScale();
COLOR=mc-1;
for(x=0;x<mx;x++)
	if( x%(int)jedenx==0 ) BiosLine(x,0,x,10);
for(y=0;y<my;y++)
	if( y%(int)jedeny==0 ) BiosLine(0,y,10,y);
return;
}

void detailsBIN(void)
{
register int y=0;
register int x=0;
register int c=0;
double xr=0.0,yr=0.0;
fprintf(trout,"BIN 256:\n");
   for(y=0;y<my;y++)
    {
    for(x=0;x<mx;x++)
     //if((x-1)%5==0 && (y-1)%5==0)
     //{
    //	fprintf(trout,"%c",BiosGetPixel(x,y));
    //	}
    //	else
	{
	 BiosSetPixel(x,y,x);
	 xr=(double)x/jedenx+XS;
	 yr=(double)y/jedeny+YS;
	 c=N*lapunBIN(xr,yr);
	 fprintf(trout,"%c",c%256);
	 c=-(c%mc);
	 BiosSetPixel(x,y,c);
	 if(kbhit() && getch()==27 ) return;
	}
    fprintf(trout,"\n");
    fflush(trout);
    }
return;
}

void closing(void)
{
if(trout!=NULL){
              fprintf(trout,"\nEND");
              fclose(trout);
              }
BiosScreen(0x3);
return ;
}

void init(void)
{
static char flag=0;
static twochar pom;
static unsigned int mod=0x3;

char tab[80],*end,command;

if(!flag)
{
pom=BiosDevices();
if(pom.one>7)
	mod=0x13;
	else
	mod=0x4;
BiosScreen(mod);
flag=1;
my=MAXY;
mx=MAXX;
mc=NUMCOLOR;
}

BiosScreen(0x3);
BEGIN:
printf("\n               VGA - LAPUNOW                              \n"
       "------------------------------------------------------------\n");
printf("\t Primary card: %s\n\t Secondary card : %s\n\n",cards[(pom.one!=0xFF?pom.one:0)],
							 cards[(pom.two!=0xFF?pom.two:0)]);
do{
printf(" RYSOWANIE FUNKCJI LAPUNOWA PRZY ZMIENNEJ STRATEGII -Q wyjscie\n");
printf(" T - tryb graficzny wg BIOS -> %d \n ",mod);
printf(" rozdzielczosc ekranu:  %d / %d pixeli \n %d kolorow\n",mx,my,mc);
printf(" x..X -> %g..%g \n y..Y -> %g..%g \n Liczba iteracji -> %d \n Pattern -> %s[%d]\n Zbior wyjsciowy -> %s\n",
XS,X,YS,Y,NUMBER,pattern,np,outputname);
printf("\n Wcisnij T,X,x,Y,y,L,P,Z,Q !\n");
printf(" Klawisz z tylda (~) rozpoczyna obliczenia !\n");
command=getch();

switch (command) {
case 'q':
case 'Q':exit(0);
case 'X':putchar('?');
	 scanf("%f",&X);break;
case 'Y':putchar('?');
	 scanf("%f",&Y);break;
case 'x':putchar('?');
	 scanf("%f",&XS);break;
case 'y':putchar('?');
	 scanf("%f",&YS);break;
case 'l':
case 'L':putchar('?');gets(tab);NUMBER=atof(tab);  break;
case 'p':
case 'P':putchar('?');gets(tab);strcpy(pattern,tab);break;
case 'z':
case 'Z':putchar('?');gets(tab);strcpy(outputname,tab);break;
case 'T':
case 't':
case 'm':
case 'M':putchar('?');
	 gets( tab );
	 mod=strtol( tab, &end , 0 );
	 BiosScreen(mod);
         if( !BiosScreen( mod ))
		{
		BiosScreen(0x3); /* VGA 25/80 text mode */
		fprintf(stderr,"Nie ma takiego trybu! Wcisnij ENTER\n");
                gets(tab);
		}
                else
                {
	        my=MAXY;
	        mx=MAXX;
	        mc=NUMCOLOR;
	        BiosScreen(0x3);
                }
                break;
default:putchar(BEEP);break;
}
np=strlen(pattern);
clrscr();
}while(command!='`');

   if ((trout = fopen(outputname, "wt")) == NULL)
   {
      fprintf(stderr, "Cannot open output \
	      file.\n");
      exit(1);
   }
/*
printf(" BIOS Mode ? \n"
       " Put number (if hexadecimal -> 0xNN )\n");

*/
printf(" Tryb %d \n czyli %xH\n", mod , mod  );
printf(" Wcisnij ENTER \n");
gets( tab );
if( !BiosScreen( mod ))
		{
		BiosScreen(0x3); /* VGA 25/80 text mode */
		fprintf(stderr,"Invalid Mode! = Bledny tryb!");
		goto BEGIN;
		}
		else
		{
		my=MAXY;
		mx=MAXX;
		mc=NUMCOLOR;
		if(mc>16)
			{
			N=64;
			mc=256;
			}
		    else
		    {
		    N=4;
		    mc=16;
		    }
		np=strlen(pattern);
		jedenx=(double)mx/(X-XS);
		jedeny=(double)my/(Y-YS);
		}

if ((trout = fopen(outputname, "wt"))
       == NULL)
   {
      fprintf(stderr, "Cannot open output \
	      file.\n");
      exit(1);
   }

  fprintf(trout,"BEGIN\n maxX=%d\n maxY=%d\n ",mx,my);

  fflush(trout);
  return ;
/* end initialization */
}

int main(void)
{
  atexit(closing);
  signal(SIGFPE,FpCatcher);
  if(setjmp(fpjumper)!=0)
	{
	printf("FP error !\n");
	getch();
	}
  ctrlbrk(BrCatcher);
  if(setjmp(brjumper)!=0)
	{
	printf("User break !\n");
	getch();
	}
do{
  init();
  schem();
  detailsBIN();
  while(getch()!=27)
                    ;
}while(1);

return 0;
}
