/*
 Funkcje VESA bios extension
*/
#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include <DOS.h>
#include <math.h>
#include <conio.h>
#include "BIOSGRAF.h"
#include "VESA.HPP"

unsigned char far *WINA=(unsigned char far *)MK_FP(0xA000,0x0000);
unsigned char far *WINB=(unsigned char far *)MK_FP(0xA000,0x0000);

unsigned char ATR_A=0x00;
unsigned char ATR_B=0x00;
unsigned int BANKINA=0;
unsigned int BANKINB=0;

unsigned char BANK_NUM=1;
unsigned int MULTIBANK=1;// o ile trzeba mnozyc dla okien co 64KB
unsigned long int GRANULARITY=0xffff;
unsigned long int  WIN_SIZE=0xffff;

void far MyAbort()
{
fprintf(stderr,"No Banking !\n");
BANKINA=0;
}

void far (*ChangeBank)()=MyAbort;


int far IsAvailable(unsigned mode)
{
int i=0;
while(LISTATRYBOW[i]<0xffff)
	{
	if(LISTATRYBOW[i]==mode) return 1;
	i++;
	}
return 0;
}

int far VesaScreen(unsigned mode)
{
if(!IsAvailable(mode) )return 0;
_BX=mode;
_AX=0x4f02;
Int10H();
char pomL=_AL;
char pomH=_AH;
if(pomH==1 || pomL!=0x4F) return 0;
	else
	{
	MODE=mode;

	return 1;
	}
}

extern unsigned int COLS;
void far VesaSetParameters()
{
MODEinfo b;

_CX=MODE;
_ES=FP_SEG(&b);
_DI=FP_OFF(&b);
_AX=0x4F01;
geninterrupt(0x10);

char pomL=_AL;
char pomH=_AH;
if(pomH==0x01 || pomL!=0x4F)
	return ;
GRAPH=(b.A_mode & 0x0010);
pomL=0xFF;
pomL<<=b.BitsPoint;
NUMCOLOR=(unsigned char)(pomL^0xff)+1;
COLS =*(unsigned int far*)MK_FP(0x0040,0x004A);/* Czytane z obszaru zmiennych biosu */
//COLS=b.LineBytes;
if(GRAPH)
	{
	MAXX=b.Xsize-1;
	MAXY=b.Ysize-1;
	LINELEN=MAXX+1;
	TMAXX=b.Xsize/b.CharHoriz-1;
	TMAXY=b.Ysize/b.CharVert-1;
	if(MODE>0x10C && b.BitsPoint==16)
		{
		/* TRIDENT */
		MAXX=2*b.Xsize-1;
		TMAXX=2*b.Xsize/b.CharHoriz-1;
		}
	}
	else
	{
	MAXX=b.Xsize*b.CharHoriz-1;;
	MAXY=b.Ysize*b.CharVert-1;;
	TMAXX=b.Xsize-1;
	TMAXY=b.Ysize-1;
	LINELEN=TMAXX*2;
	}

CHARMATTRIX=b.CharHoriz;
SCREEN=WINA=(unsigned char far*)MK_FP(b.SegA,0x0000);
       WINB=(unsigned char far*)MK_FP(b.SegB,0x0000);

ATR_A=b.A_win_A;
ATR_B=b.A_win_B;
GRANULARITY=b.Granularity*1024;/* Banki zaczynaja sie co iles KB */
/* Przeliczenie na banki 64KB */
MULTIBANK=64/b.Granularity;
WIN_SIZE=(long)b.WindSize*1024;
ChangeBank=b.SwithProc;
if(NUMCOLOR>16)
	BANK_NUM=((long)b.Ysize*b.Xsize*b.BitsPoint/8+0xfffe)/0xffff;
	else
	BANK_NUM=b.BankNum;
BANKINA=-1;
BANKINB=-1;
}


