#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>



void koloRN(int xs,int ys,int r)
{int y,x,xa,xe,end;
long int r2,y2;
r2=r*r;
for(y=0;y!=r;y++)
	{
	y2=(long int)y*y;
	x=sqrt(r2-y2);
	xe=xs+x;
	for(xa=xs-x;xa<xe;xa++)
		{
		putpixel(xa,ys+y,4);
		putpixel(xa,ys-y,5);
		}
	}
}

void kolo(int xs,int ys,int b)
{int y,x,xa,xe,a,end,color;
long int y2,a2,b2;
a=b*1.33;
b2=(long int)b*b;
a2=(long int)a*a;
for(y=0;y<b;y++)
	{
	y2=(long int)y*y;
	x=sqrt((a2*b2-y2*a2)/b2);
	xe=xs+x;
	color=random(getmaxcolor())+1;
	for(xa=xs-x;xa<xe;xa++)
		{
		putpixel(xa,ys+y,color);
		putpixel(xa,ys-y,color);
		}
	}
}

int main(void)
{
   /* request auto detection */
   int gdriver = DETECT, gmode, errorcode,i;

   /* initialize graphics mode */
   initgraph(&gdriver, &gmode, "c:/biobit/bgi");

   /* read result of initialization */
   errorcode = graphresult();

   if (errorcode != grOk)  /* an error occurred */
   {
      printf("Graphics error: %s\n", grapherrormsg(errorcode));
      printf("Press any key to halt:");
      getch();
      exit(1);             /* return with error code */
   }
setgraphmode(1);
   /* draw a line */
   randomize();
   for(i=1;i>0;i++)
		{
		errorcode=random(110);
	   kolo(random(getmaxx()),random(getmaxy()),errorcode);
	   setfillstyle(CLOSE_DOT_FILL,1);
	   setcolor(0);
	   fillellipse(random(getmaxx()),random(getmaxy()),errorcode,errorcode);
	   if(kbhit()) break;
		}
   /* clean up */
   getch();
   closegraph();
   return 0;
}
