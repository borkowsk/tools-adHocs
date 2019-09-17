/*
test WUI - procedury obslugi ekranu
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <values.h>
#include <math.h>
#include <conio.h>

#include "biosgraf.h"
#include "picture.hpp"
#include "screens.hpp"
#include "fonts.hpp"
#include "window.hpp"

#define BREAK if(kbhit()){getch();break;}

void error(char* p,int c);



extern unsigned char far image[32][32];

const mem_picture I((pixpointer)image,31,31);
mem_picture B(34,44);
void ColorTest(picture& S);

void Test(picture& S)
{
#define MAXX S.GetMaxX()
#define MAXY S.GetMaxY()

int i,j;
if( S.GetMaxX() < 10)
	return;
Window W(S,S.GetMaxX()/4,S.GetMaxY()/4,S.GetMaxX()/2,S.GetMaxY()/2);
S.Clear();
ColorTest(S);
S.SetColor(15);
delay(1000);
S.Clear();
S.FillRectAngle(MAXX/3,0,MAXX/2,MAXY);
for( j=0 ; j < MAXY ; j++ )
	S.PutPixel( j , j, 15 );
for( j=0 ; j < MAXY ; j++ )
	S.PutPixel( j , j+1, 0 );
S.DrawLine(0,0,MAXX,MAXY);
S.DrawLine(0,MAXY,MAXX,0);
for( j=0 ; j < MAXX ; j++ )
	S.PutPixel( j , j, 15 );
for( j=0 ; j < MAXX; j++ )
	S.PutPixel( j , j+1, 0 );

for(i=0;i<(MAXY/2);i+=10)
	{
	S.SetColor(random(16));
	S.DrawEllipse(MAXX/2,MAXY/2,i,i);
	BREAK;
	}
for(i=0;i<(MAXY/2);i++)
	{
	S.SetColor(random(16));
	S.DrawCircle(MAXX/2,MAXY/2,i);
	BREAK;
	}
for( i=0 ; i <= MAXX; i++ )
	{
	for( j=0 ; j <= MAXY ; j++ )
		S.PutPixel( i , j, i );
	BREAK;
	}
for( j=1 ; j < 10 ; j++ )
	{
	S.ClearBar( random(MAXX) , random(MAXY),random(MAXX) , random(MAXY) );BREAK;
	}
for( j=1 ; j < 10 ; j++ )
	{
	S.SetColor(random(256));
	S.GetPalette().Lock(S.GetColor());
	S.FillRectAngle(random(MAXX) , random(MAXY),random(MAXX) , random(MAXY)  );BREAK;
	}
for( j=1 ; j < MAXY*5 ; j++ )
	{
	S.SetColor(random(256));
	S.GetPalette().Lock(S.GetColor());
	S.DrawLine(random(MAXX) , random(MAXY),MAXX/2, MAXY/2 );BREAK;
	}
for( j=1 ; j < MAXY/2 ; j++ )
       {
       S.SetColor(random(256));
       S.GetPalette().Lock(S.GetColor());
       S.FillCircle( random(MAXX) , random(MAXY), random(MAXX/5) );BREAK;
       }
for(  j=1 ; j < MAXY/2 ; j++ )
       {
       S.SetColor(random(256));
       S.GetPalette().Lock(S.GetColor());
       S.FillEllipse( MAXX/2-(j*2) , MAXY/2 , j, j );
       S.FillEllipse( MAXX/2+(j*2) , MAXY/2 , j, j );
       S.FillEllipse( MAXX/2 , MAXY/2-(j*2) , j, j );
       S.FillEllipse( MAXX/2 , MAXY/2+(j*2) , j, j );
       BREAK;
       }

for(i=0;i<MAXX;i+=1)
	{
	S.PutPicture(MAXX-i,i-(0.5*i),I);
	BREAK;
	}
S.PutPicture(-10,-10,I);
S.PutPicture(-10,MAXY-20,I);
S.PutPicture(MAXX-20,-10,I);
S.PutPicture(MAXX-20,MAXY-20,I);
S.SetColor(16);
B.GetPicture(S,MAXX/4,MAXY/4);
S.PutPicture(MAXX/4,MAXY/4,I);
delay(200);
S.PutPicture(MAXX/4,MAXY/4,B);
delay(200);
for(i=0;i<MAXX-100;i+=1)
	{
	S.PutPicture(i,50+i-(0.5*i),B);
	BREAK;
	}
W.Clear();
Test(W);
delay(500);
S.SetBackground(1);
int skok=S.GetMaxY()/100;
if(skok<=0)
	skok=1;
for(i=1;i<=(S.GetMaxY()+1)/skok;i++)
	{
	S.ShiftDown(skok);
	S.ClearBar(0,(i-1)*skok,S.GetMaxX(),i*skok);
	BREAK;
	}
#undef MAXX
#undef MAXY
}
