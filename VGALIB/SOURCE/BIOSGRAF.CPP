/*
BIOS-owa obsluga karty graficznej - procedury podstawowe
*/
#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include <DOS.h>
#include <math.h>
#include <conio.h>
#include "BIOSGRAF.h"
#include "VESA.HPP"
/*
Funkcja biosu 10H niszczy rejestry DI i SI zawierajace zmienne rejestrowe
*/
#pragma options -r-

struct MODES { signed char GRAPH;
	       int  SEG;
	       int  maxX;
	       int  maxY;
	       int  Xrat;
	       int  Yrat;} MODES[]=
	      {{0,0xB800,40,25,1,1},/* 0 */
	       {0,0xB800,40,25,1,1},/* 1 */
	       {0,0xB800,80,25,1,1},/* 2 */
	       {0,0xB800,80,25,1,1},/* 3 */
	       {1,0xB800,320,200,8333,10000},/* 4 */
	       {1,0xB800,320,200,8333,10000},/* 5 */
	       {1,0xB800,320,200,4300,10000},/* 6 */
	       {0,0xB000,80,25,1,1},/* 7 */

	       {-1,0XB800,80,25,1,1},/* 8 */
	       {-1,0XB800,80,25,1,1},/* 9 */
	       {-1,0XB800,80,25,1,1},/* A */
	       {-1,0XB800,80,25,1,1},/* B */
	       {-1,0XB800,80,25,1,1},/* C */

	       {1,0XA000,320,200,8333,10000},/* D */
	       {1,0XA000,640,200,4300,10000},/* E */
	       {1,0XA000,640,350,7500,10000},/* F */
	       {1,0XA000,640,350,7500,10000},/*10 */
	       {1,0XA000,640,400,1,1},	     /*11 */
	       {1,0XA000,640,400,1,1},	     /*12 */
	       {1,0XA000,320,200,8333,10000},/*13 */
	       {1,0XA000,320,400,83,50}	 /*320X400*/
	       };

unsigned char far* SCREEN = (unsigned char far*)MK_FP( 0xB800 , 0x0000 );
unsigned char DEVICE = 0;

unsigned int  MODE = 0;
unsigned char GRAPH= 0;
unsigned char PAGE = 0;
unsigned char NUMPAGE = 0;

unsigned int COLS =*(unsigned int far*)MK_FP(0x0040,0x004A);/* Czytane z obszaru zmiennych biosu */
unsigned char far& BIOSMODE=*(unsigned char far*)MK_FP(0x0040,0x0049);/* Czytane z obszaru zmiennych biosu */
unsigned long int LINELEN=1024;

unsigned int MAXX=1023;
unsigned int MAXY=1023;
unsigned int TMAXX=39;
unsigned int TMAXY=24;

unsigned int CHARMATTRIX=8;
unsigned int XRATIO= 8333;
unsigned int YRATIO=10000;
unsigned int RATIOXY=1; /* XRATIO/YRATIO */

unsigned int COLOR=1;
unsigned int NUMCOLOR=1;
unsigned char PUTMODE=0; /* 0/1	->	OR/XOR mode */



void PrintModeInfo()
{
printf("INFORMATION ABOUT SCREEN MODE:\n"
"SCREEN ADRES %Fp Graphics: %c \n"
"MODE %u - %xH \n"
"ACTIVE PAGE %u\n"
"NUMBER OF PAGES %u\n"
"TEXT X 1..%u \n"
"TEXT Y 1..%u \n"
"BIOS COLUMNS %u\n"
"CHAR MATTRIX HIGHT %u\n"
"MAXX %u PIX.\n"
"MAXY %u PIX.\n"
"NUMBER OF COLOR %u ",
SCREEN,
(GRAPH?'Y':'N') ,
MODE   , MODE ,
PAGE    ,
NUMPAGE  ,
TMAXX+1  ,
TMAXY+1  ,
COLS     ,
CHARMATTRIX  ,
MAXX+1   ,
MAXY+1   ,
NUMCOLOR);
if(NUMCOLOR==256)
	printf(" or MONO - 0\n");
	else
	putchar('\n');
if(GRAPH)
printf("XRATIO/YRATIO : %d/%d \n",XRATIO,YRATIO);
}


char *cards[]={"Unknown or nothing","MDA mono or HGC","CGA color",
	       "3-reserved","EGA color","EGA mono",
	       "PGC","VGA mono","VGA color",
	       "9-reserved"};

twochar BiosDevices()
{
twochar pom;
unsigned char test;
_AH=0x1A;
_AL=0x0;
geninterrupt(0x10);
test=_AL;
if( test==0x1A )
	{
	pom.one=DEVICE=_BL;
	pom.two=_BH;
	}
     else
	{
	pom.one=0xFF;
	pom.two=0xFF;
	DEVICE=0;
	}
{
VESAinfo smietnik;
if(DEVICE>=7) IsVesa(&smietnik);/* Test czy jest rozszerzenie */
}
return pom;
}

int IsGraph()
{
BiosPage(0);
BiosSetPixel(10,10,1);
return BiosGetPixel(10,10)==1;
}

void CheckScreenSize()
{
BiosSetPixel(1,1,1);
BiosSetPixel(1,319,1);
BiosSetPixel(199,1,1);
BiosSetPixel(199,319,1);

BiosSetPixel(0,639,2);
if(BiosGetPixel(1,319)==1)
	MAXX=319;
	else
	MAXX=639;
}

void ReadAndSetParameters()
{
unsigned char bufor[64]; /* bufor na dane z przerwania */
COLS =*(unsigned int far*)MK_FP(0x0040,0x004A);/* Czytane z obszaru zmiennych biosu */
GRAPH=IsGraph();
if(DEVICE<7)
	{
	TMAXX=COLS-1;
	if(GRAPH )
		{
		MAXY=MODES[MODE].maxY-1;
		MAXX=MODES[MODE].maxX-1;
//		CheckScreenSize();
		TMAXY=24;
		LINELEN=MAXX+1;
		COLOR=1;
		NUMCOLOR=16;
		}
		else
		{
		TMAXY=MODES[MODE].maxY-1;
		MAXY=MODES[MODE].maxY*8;
		MAXX=MODES[MODE].maxX*8;
		}
	return;
	}
for(int i=0;i<64;i++) bufor[i]=0;/* jest zerowany */
_ES=FP_SEG((char far*)bufor);
_DI=FP_OFF((char far*)bufor);
_BX=0;
_AH=0x1B;
/*
Funkcja biosu 10H niszczy rejestry DI i SI zawierajace zmienne rejestrowe
*/
Int10H();
MODE = bufor[0x4];
PAGE = bufor[0x1D];
NUMPAGE = bufor[0x29];
TMAXX=bufor[5]-1;/**((unsigned int*)bufor+5)*/;
TMAXY=bufor[0x22]-1;
CHARMATTRIX=bufor[0x23]/**((unsigned int*)bufor+23)*/;
MAXX=(TMAXX+1)*8-1;
LINELEN=MAXX+1;
MAXY=(TMAXY+1)*CHARMATTRIX-1;
COLOR=1;
if(bufor[0x27])
	NUMCOLOR=bufor[0x27];
	else
	NUMCOLOR=256;
PUTMODE=0; /* 0/1	->	OR/XOR mode */
}
#define MISC_OUT_REG   0x3c2
#define GRAPH_CTRL     0x3ce
#define MISC_REG       0x006
#define START_ADR_HI  0x00c
#define CRT            0x3d4
#define MAX_SCAN_LINE  0x009

void enable128KB();
void setCRTto400();
void setactivepage320x400(char n);
void setvisualpage320x400(char n);
void setstart320x400(unsigned int n);


int  far  BiosScreen( unsigned mode )
{
BiosDevices();
if(ISVESA && mode>=0x100 )
	{
	if(!VesaScreen(mode))
			return 0;
	VesaSetParameters();
	goto CHECKRATIO;
	}
_AH=0x00;
if(mode==0x14) _AL=mode-1;
	else _AL=mode;
geninterrupt(0x10);
_AH=0x0F;
geninterrupt(0x10);
MODE=_AL;
COLS=_AH;
PAGE=_BH;
TMAXX=COLS-1;

if(mode != MODE )
	if(mode !=0x14) return 0;
	    else
	    if(MODE !=0x13) return 0;
	       else
	       {
	       MODE=0x14;
	       enable128KB();
	       setCRTto400();
	       setactivepage320x400(0);
	       setvisualpage320x400(0);
	       }
ReadAndSetParameters();
 /* Odpowiednia funkcja biosu dziala tylko na VGA */
 /* na innych kartach sa proby odczytania innymi metodami */

if( mode<=0x14 )
	{
	SCREEN = (unsigned char far*)MK_FP( MODES[mode].SEG , 0x0000 );
	if(MODES[mode].GRAPH>=0)
		GRAPH = MODES[mode].GRAPH;
		else
		GRAPH =0;
	XRATIO=MODES[mode].Xrat;
	YRATIO=MODES[mode].Yrat;
	if(mode==0x14) MAXY=399;/* Dodany tryb 320x400 */
	return 1;/* Tryb standartowy */
	}


if(!GRAPH)
	SCREEN = (unsigned char far*)MK_FP( 0xB800 , 0x0000 );
	else
	SCREEN = (unsigned char far*)MK_FP( 0xA000 , 0x0000 );

CHECKRATIO: /* Ustawienie X i Y RATIO */
if(MAXX==319)
	{
	if(MAXY==199)
		{
		XRATIO= 8333;
		YRATIO=10000;
		}
	if(MAXY==399)
		{
		XRATIO=8333;
		YRATIO=5000;
		}
	}
else if(MAXX==639)
	{
	if(MAXY==349)
		{
		XRATIO= 755;
		YRATIO=1000;
		}
	if(MAXY==399)
		{
		XRATIO= 8333;
		YRATIO=10000;
		}
	if(MAXY==479)
		{
		XRATIO=YRATIO=1;
		}
	}
	else
	XRATIO=YRATIO=1;
return 1;
}

void EditParameters()
{
char pom[10];
do{
gotoxy(1,1);
PrintModeInfo();
printf("OK ? q/a/g/x/y \n"),
gets(pom);
if(*pom=='g')
	{
	putchar('?');
	gets(pom);
	if(*pom=='y') GRAPH=1;
		else  GRAPH=0;
	 }
if(*pom=='a')
	{
	unsigned pomL;
	putchar('?');
	gets(pom);
	pomL=strtol( pom, NULL , 0 );
	SCREEN = (unsigned char far*)MK_FP( pomL , 0x0000 );
	}
if(*pom=='x')
	{
	long pomL;
	putchar('?');
	gets(pom);
	pomL=strtol( pom, NULL , 0 );
	XRATIO =pomL;
	}
if(*pom=='y')
	{
	long pomL;
	putchar('?');
	gets(pom);
	pomL=strtol( pom, NULL , 0 );
	YRATIO =pomL;
	}
}while(*pom!='q');
}


void far BiosCleardevice()
{
_CH=0;
_CL=0;
_DH=80;
_DL=80;
_BH=0;
_AL=0;
_AH=0x7;
geninterrupt( 0x10 );
}

void  far BiosPage( unsigned char LPAGE )
{
_AL=LPAGE;
_AH=0x5;
Int10H();
PAGE=LPAGE;
}

void  far BiosSetPROB( unsigned char val )
/* Example - writing the value val to PROB using BIOS */
{
_BH=0x0A;
_BL=val;
_AX=0x007F;
geninterrupt(0x10);
}

void  far BiosSetPROA( unsigned char val )
{
_BH=0x09;/* val to PROA */
_BL=val;
_AX=0x007F;
geninterrupt(0x10);
}

void  far BiosSetPixel( long int x , long int y ,unsigned int color )
/* ustawienie pixela wywolaniem INT 10 funkcja 12 */
{
int xx=x;
int yy=y;
if(xx<0 || yy<0 || xx>MAXX || yy>MAXY ) return;
_CX=xx;
_DX=yy;
_BH=PAGE;
_AL=color;
_AH=0x0C;/* Funkcja 12 */
geninterrupt(0x10);
}

unsigned int  far BiosGetPixel( long int x , long int y )
/* odczytanie pixela wywolaniem INT 10 funkcja 13 */
{
int xx=x;
int yy=y;
_CX=xx;
_DX=yy;
_BH=PAGE;
_AH=0x0D;/* Funkcja 13 */
geninterrupt(0x10);
xx=_AL;
return xx;
}

void setstart320x400(unsigned int n)
{asm{
	mov dx,CRT
	mov al,START_ADR_HI
	mov bx, n ;//Ladowanie parametru
	mov ah,bh
	out dx,ax
	inc al
	mov ah,bl
	out dx,ax
}}

void setvisualpage320x400(char n)
{asm{
	mov al,n
	cbw
	shl ax,15d
}
setstart320x400(_AX);
}

void setactivepage320x400(char n)
{asm{
	mov al,63h
	mov bl,n
	and bl,1
	shl bl,5
	sub al,bl;//ktora polowa 128KB ma byc uaktywniona
	mov dx,MISC_OUT_REG
	out dx,al
}}
void enable128KB()
{asm{
	mov dx,GRAPH_CTRL
	mov al,MISC_REG
	out dx,al
	inc dx
	mov al,1
	out dx,al
}}

void setCRTto400()
{asm{
	mov dx, CRT
	mov al, MAX_SCAN_LINE
	out dx,al
	inc dx
	in al,dx
	and al,70H
	out dx,al
}}

void linecopy( void far* p,void far* b,unsigned len )
{   asm{
	push ds
	cld;
	mov cx,len
	lds si,b
	les di,p
     }
POCZ:asm{
	lodsb
	or al,0
	jnz ROB
	inc di
	loop POCZ
	jmp END
	}
ROB:asm{
	stosb
	loop POCZ
	}
END:asm{
	pop ds
	}
}

