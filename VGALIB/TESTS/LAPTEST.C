#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <graphics.h>
#define BEEP '\006'
/* stale dokladnosci liczenia */
#define MAX_VAL 3.4e48
#define MIN_VAL -3.4e48

int NUMBER=500;
char outputname[100]="null";/* nazwa zbioru z zapisem wynikow */
char pattern[100]="10";/* wzor strategi w funkcji lapunowa */
int np=sizeof(pattern)-1;/* i jego rozmiar */

double jedenx=0.0, jedeny=0.0;/* inicjacja jest zmieniana w init ! */
int my=0,mx=0,mc=0;
FILE* trout;

double lapunBIN(double a, double b)
/* szacowanie funkcji lapunowa przy zmiennej strategi */
{
double total=0.0,wx=0.0,aktr=0.0,aktrWx=0.0,logz2=0.0;
register int  i=0;
register char* p=pattern;

logz2=log10(2.0);
wx=0.0000000001;
if(a==0.0) return 1.0;
if(b==0.0) return 1.0;
for(i=1;i<=NUMBER;i++)
	{
       if (total>MAX_VAL) break;
       if (total<MIN_VAL) break;

       if(p[i%np]=='0') aktr=a;
		   else aktr=b;

       /* x <- rx(1-x) funkcja logistyczna */

       aktrWx=aktr*wx;
       wx=aktrWx*(1-wx);

       if (wx>MAX_VAL) break ;
       if (wx<MIN_VAL) break ;

       aktr=fabs(aktr-2.0*aktrWx);

       if (aktr>MAX_VAL) break ;
       if (!(aktr>0)) break ;

       aktr=log10(aktr);

       if (aktr>MAX_VAL) break ;
       if (aktr<MIN_VAL) break ;

       aktr=aktr/logz2;

       if (aktr>MAX_VAL) break ;
       if (aktr<MIN_VAL) break ;

       total=total+aktr;

	}
if((i-=1)>0){
	     if((total/=i)>0) return 0.0;
			 else return total;
	     }
	     else return MAX_VAL;
/*end lapunow*/
}



void schem(void)
{
register int y=0;
register int x=0;
register int c=0;
double xr=0.0,yr=0.0;

   for(y=1;y<my;y+=10)
    for(x=1;x<mx;x+=10)
	{
	 putpixel(x,y,1);
	 xr=(double)x/jedenx;
	 yr=(double)y/jedeny;
	 c=lapunBIN(xr,yr);
	 c=c%mc;
	 putpixel(x,y,c);
	 if(kbhit()) exit(2);
	}
return;
}

void detailsBIN(void)
{
register int y=0;
register int x=0;
register int c=0;
double xr=0.0,yr=0.0;
fprintf(trout,"BIN 256:\n");
   for(y=1;y<my;y++)
    {
    for(x=1;x<mx;x++)
	{
	 putpixel(x,y,1);
	 xr=(double)x/jedenx;
	 yr=(double)y/jedeny;
	 c=lapunBIN(xr,yr);
	 fprintf(trout,"%c",c%256);
	 c=c%mc;
	 putpixel(x,y,c);
	 if(kbhit()) exit(2);
	}
    fprintf(trout,"\n");
    fflush(trout);
    }
return;
}

void closing(void)
{
fprintf(trout,"\nEND");
fclose(trout);
closegraph();
return ;
}

void init(void)
{
int gdriver = DETECT, gmode,maxgmode,errorcode;
int far* pgmode;
char far* gdrname;
char bufor[1000];
char command;
double X=10.0,Y=8.0;

pgmode=(int far*)&gmode;
initgraph(&gdriver,pgmode,"c:\\br\\bgi");
errorcode = graphresult();

   if (errorcode != grOk)
   {
      printf("Graphics error: %s\n", grapherrormsg(errorcode));
      printf("Press any key to halt and move driver to this directory !");
      getch();
      exit(1);
   }

gdrname=getdrivername();
maxgmode=getmaxmode();
my=getmaxy();
mx=getmaxx();
mc=/*getmaxcolor()+*/2;
np=strlen(pattern);

if ((trout = fopen(outputname, "wt"))
       == NULL)
   {
      fprintf(stderr, "Cannot open output \
	      file.\n");
      exit(1);
   }
  atexit(closing);


  jedenx=(double)mx/X;
  jedeny=(double)my/Y;

  fprintf(trout,"BEGIN\n maxX=%d\n maxY=%d\n ",mx,my);

  fflush(trout);
  return ;
/* end initialization */
}



int main(void)
{
  init();
  schem();
  detailsBIN();
              
   /* clean up */
   getch();
   return 0;
}
