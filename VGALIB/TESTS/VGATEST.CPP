/*
Test trybow graficznych karty TopVGA
*/

#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <mem.h>
#include <stdlib.h>
#include <string.h>
#include "BIOSGRAF.h"
#include "BIOSCHAR.hpp"
#include "VESA.HPP"
#include "TopVGA.h"

#define BREAK if(kbhit()){getch();break;return;}
#define MEMSET _fmemset
extern unsigned char far image[][32];
unsigned oldmode;
VESAinfo b;

void TestPalette13H()
{
RGBrec pom;
int krokX=MAXX/16;
int krokY=MAXY/16;
int i,j;
int startX=0,startY=0;

for(i=0;i<8;i++)
  for(j=0;j<8;j++)
	{
	pom.I=i;pom.II=j;pom.III=0;
	COLOR=GetColorNum(pom);
	FillBar13H(startX+i*krokX,startY+j*krokY,startX+(i+1)*krokX,startY+(j+1)*krokY);
	}
startX=8*krokX;
for(i=0;i<8;i++)
  for(j=0;j<8;j++)
	{
	pom.I=i;pom.II=j;pom.III=1;
	COLOR=GetColorNum(pom);
	FillBar13H(startX+i*krokX,startY+j*krokY,startX+(i+1)*krokX,startY+(j+1)*krokY);
	}
startY=8*krokY;
startX=0;
for(i=0;i<8;i++)
  for(j=0;j<8;j++)
	{
	pom.I=i;pom.II=j;pom.III=2;
	COLOR=GetColorNum(pom);
	FillBar13H(startX+i*krokX,startY+j*krokY,startX+(i+1)*krokX,startY+(j+1)*krokY);
	}
startY=8*krokY;
startX=8*krokX;
for(i=0;i<8;i++)
  for(j=0;j<8;j++)
	{
	pom.I=i;pom.II=j;pom.III=3;
	COLOR=GetColorNum(pom);
	FillBar13H(startX+i*krokX,startY+j*krokY,startX+(i+1)*krokX,startY+(j+1)*krokY);
	}

}

void VGAcolorstest()
{
TestPalette13H();
getch();
ColorContinum(RG_B);
getch();
ColorContinum(BR_G);
getch();
ColorContinum(GB_R);
getch();
FuncContinum();
getch();
for(int i=0;i<256;i++)
	{
	COLOR=i;
	line(i,0,i,MAXY);
	}

getch();
}

void VESABankTest()
{
int i;
if(!ISVESA) return;
if(!BiosScreen(0x101))
	if(!BiosScreen(0x100)) return; /* 640x400x256c or higher*/
ColorContinum(BR_G);
AssignBank(0,0);
MEMSET( SCREEN , 10 ,0xffff);
SCREEN[0xffff]=255;

AssignBank(0,1);
MEMSET( SCREEN , 20 ,0xffff);
SCREEN[0xffff]=255;

AssignBank(0,2);
MEMSET( SCREEN , 40 ,0xffff);
SCREEN[0xffff]=255;

AssignBank(0,3);
MEMSET( SCREEN , 50 ,0xffff);
SCREEN[0xffff]=255;

AssignBank(0,4);
MEMSET( SCREEN , 60 ,0xffff);
SCREEN[0xffff]=255;
AssignBank(0,5);
MEMSET( SCREEN , 70 ,0xffff);
SCREEN[0xffff]=255;
getch();
BiosPage(1);
getch();
BiosPage(0);
getch();
}

void PROATest()
{
if(!BiosScreen(0x13)) return; /* 320x200x256c */
ColorContinum(BR_G);
BiosPage(0);
BiosSetPROA(0);
MEMSET( SCREEN , 10 ,4096);
SCREEN[4096]=255;

BiosSetPROA(1);
MEMSET( SCREEN , 20 ,4096);
SCREEN[4096]=255;

BiosSetPROA(2);
MEMSET( SCREEN , 40 ,4096);
SCREEN[4096]=255;

BiosSetPROA(3);
MEMSET( SCREEN , 50 ,4096);
SCREEN[4096]=255;

BiosSetPROA(4);
MEMSET( SCREEN , 60 ,4096);
SCREEN[4096]=255;

BiosSetPROA(15);
MEMSET( SCREEN , 70 ,4096);
SCREEN[4096]=255;
BiosSetPROA(16);
MEMSET( SCREEN , 80 ,4096);
SCREEN[4096]=255;
BiosSetPROA(17);
MEMSET( SCREEN , 90 ,4096);
SCREEN[4096]=255;
getch();
BiosPage(1);
getch();
BiosPage(0);
getch();
}

void DemoVGA13H()
{
int x=MAXX/2,y=MAXY/2,r=random(32)+1,xrel=random(r)+1,yrel=random(r)+1;
unsigned char color=random(256);
do{
COLOR=color++;
r++;
r%=32;
FillCircle13H(x,y,r);
x+=xrel;y+=yrel;
if(x<0 || x>MAXX){xrel=-xrel;}
if(y<0 || y>MAXY){yrel=-yrel;}
}while(!kbhit());
}

void DemoVGA16()
{
int mod=MAXX/10;
int x=MAXX/2,y=MAXY/2,r=random(mod)+1,xrel=random(r)+1,yrel=random(r)+1;
unsigned char color=random(16);
do{
COLOR=color++;
r++;
r%=mod;
FillCircle16C(x,y,r);
x+=xrel;y+=yrel;
if(x<0 || x>MAXX){xrel=-xrel;}
if(y<0 || y>MAXY){yrel=-yrel;}
}while(!kbhit());
}

void DemoVGAPRA()
{
int mod=MAXX/10;
int x=MAXX/2,y=MAXY/2,r=random(mod)+1,xrel=random(r)+1,yrel=random(r)+1;
unsigned char color=random(16);
do{
COLOR=color++;
r++;
r%=mod;
FillCirclePRA(x,y,r);
x+=xrel;y+=yrel;
if(x<0 || x>MAXX){xrel=-xrel;}
if(y<0 || y>MAXY){yrel=-yrel;}
}while(!kbhit());
}

void Test()
{
int i,j;
cleardevice();
fillbar(MAXX/3,0,MAXX/2,MAXY);
for( j=0 ; j < MAXY ; j++ )
	setpixel( j , j, 15 );
for( j=0 ; j < MAXY ; j++ )
	setpixel( j , j+1, 0 );
line(0,0,MAXX,MAXY);
line(0,MAXY,MAXX,0);
for(i=0;i<(MAXY/2);i+=10)
	{
	COLOR=random(16);
	ellipse(MAXX/2,MAXY/2,i,i);
	BREAK;
	}
for(i=0;i<(MAXY/2);i++)
	{
	COLOR=random(16);
	circle(MAXX/2,MAXY/2,i);
	BREAK;
	}
for( i=0 ; i < 320 ; i++ )
	{
	for( j=0 ; j < 200 ; j++ )
		setpixel( i , j, i );
	BREAK;
	}
for( j=1 ; j < 10 ; j++ )
	{clearbar( random(MAXX) , random(MAXY),random(MAXX) , random(MAXY) );BREAK;}
for( j=1 ; j < 10 ; j++ )
	{fillbar((COLOR=random(16),random(MAXX) ) , random(MAXY),random(MAXX) , random(MAXY)  );BREAK;}
for( j=1 ; j < 1000 ; j++ )
	{line((COLOR=random(16), random(MAXX)) , random(MAXY),MAXX/2, MAXY/2 );BREAK;}
for( j=1 ; j < 100 ; j++ )
       {fillcircle(  (COLOR=random(127),random(MAXX)) , random(MAXY), random(MAXX/5) );BREAK;}
//       {fillcircle(  (COLOR=random(127),MAXX/2) , MAXY/2, MAXX/5 );BREAK;}
for( j=1 ; j < 100 ; j++ )
	{putmap(random(MAXX),random(MAXY),32,32,(unsigned char far*)&image);BREAK;}
for( j=1 ; j < 1000 ; j++ ){putmap((MAXX/2),(MAXY/2),32,32,(unsigned char far*)&image);}
for( j=1 ; j < 100 ; j++ )
	{
	fillellipse(100,100,100,100);
	}
for( j=0 ; j < 600 ; j++ )
	setpixel( j , j, 15 );
for( j=0 ; j < 600 ; j++ )
	setpixel( j , j+1, 0 );

}

main()
{
unsigned int i,j;
char tab[80],*end;
unsigned int mod=BIOSMODE;
twochar pom=BiosDevices();
oldmode=mod;
randomize();

BEGIN:
printf("\n               VGA display BIOS service test             \n"
       "------------------------------------------------------------\n");
printf("\t Primary card: %s\n\t Secondary card : %s\n\n",cards[(pom.one!=0xFF?pom.one:0)],
							 cards[(pom.two!=0xFF?pom.two:0)]);
{

if(IsVesa(&b))
	{
	printf("\tBios extension:%c%c%c%c %u %Fs \nAvailable modes:",b.Syg[0],b.Syg[1],b.Syg[2],b.Syg[3],b.Nr,b.NazwaProd);
	i=0;
	while(b.ListaTr[i]!=0xffff){ printf("%xH ",b.ListaTr[i]); i++;}
	}
	else
	printf("\t\t\tVESA bios extension not detected\n");

}
printf(" BIOS Mode ? \n"
       " Put number (if hexadecimal -> 0xNN )\n"
       " <+>last{%d}++\n <?> scan\n"
       "\n <c> VGA colors \n <b> VESA banks \n <a> PROA register \n <q> EXIT. ",mod);

gets( tab );
if( strchr(tab,'q') ) return 0;/* Exit only then !*/
if( tab[0]=='c' )
     {
     if(BiosScreen(0x13))
		VGAcolorstest();
     BiosScreen(oldmode);
     goto BEGIN;
     }
if( tab[0]=='b' )
     {
     VESABankTest();
     BiosScreen(oldmode);
     goto BEGIN;
     }
if( tab[0]=='a' )
     {
     PROATest();
     BiosScreen(oldmode);
     goto BEGIN;
     }
if( strchr(tab,'?') )
	{
	while(!BiosScreen(++mod))
		{
		printf("Invalid mode number %d \n",mod);
		    delay(50);
		if(kbhit()) break;
		}
	}
	else
	{
	if( strchr(tab,'+') )
		mod++;
		else
		{
		mod=strtol( tab, &end , 0 );
		printf(" Tryb %d \n czyli %xH", mod , mod  );
		gets( tab );
		}
	}
if( !BiosScreen( mod ))
		{
		BiosScreen(oldmode); /* VGA 25/80 text mode */
		fprintf(stderr,"Invalid Mode !");
		goto BEGIN;
		}
		else
		{
		if((mod=MODE)<=0x13)
			{
			PrintModeInfo();
			getch();
			}
			else
			EditParameters();
		if(GRAPH) ManualSetService();
		}

{
for(i=0 ; i <=0x1000 ; i++)
	*(SCREEN+i)=i;
}

if(GRAPH)
	{
	Test();
	getch();
	}
BiosGotoXY(0,0);
if( GRAPH )
	{
	delay(200);
	for( i=0 ; i <= (TMAXX*TMAXY); i++)
		{
		COLOR=random(NUMCOLOR);
		int pom=32+random(64);
		COLOR=random(NUMCOLOR);
		BiosTTYChar(pom);
		}
	}
	else
	{
	delay(200);
	for( i=0 ; i <= ((TMAXX+1)*(TMAXY+1)); i++)
		{
		unsigned char pom=32+random(64);
		COLOR=random(256);
		BiosAChar(pom,COLOR);
		BiosTTYChar(pom);
		}
	}

getch();
BiosScreen(oldmode); /* VGA 25/80 text mode */
goto BEGIN;
}

