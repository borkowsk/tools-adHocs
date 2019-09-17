/*
BIOS-owa obsluga karty VGA - typ niby TrueColor -przestarzaly
*/
#include "biosgraf.h"
#include "color.hpp"
#include <mem.h>
#include <values.h>

RGBnode color::nodes[256];/* tablica RBG dla 256 kolorow */
byte    color::locks[256];/* tablica blokowania dla 256 kolorow */
byte 	color::Flag;
byte	color::cubea;
byte	color::cubes;

inline int abs(int w)
{
return w<0?-w:w;
}

inline long int sqr(int w)
{
return w*w;
}


inline word ColorDist(byte i,byte R,byte G,byte B)
{
//return 30*abs(color::nodes[i].R-R)+59*abs(color::nodes[i].G-G)+11*abs(color::nodes[i].B-B);
return isqrt(sqr(color::nodes[i].R-R)+sqr(color::nodes[i].G-G)+sqr(color::nodes[i].B-B));
//return abs(color::nodes[i].R-R)+abs(color::nodes[i].G-G)+abs(color::nodes[i].B-B);
}

/* Tylko n bitow rejestru RGB jest wazne */
#define MASK(R) R&=0x3F;
void color::tohardware()
/* Uzgodnienie tablicy kolorow karty z wirtualna */
{
BiosSetRGBBlock(0,256,(void far*)nodes);
}

void color::fromhardware()
/* Pobranie tablicy kolorow karty do wirtualnej */
{
BiosGetRGBBlock(0,256,(void far*)nodes);
memset(locks,1,32);/* Zablokowanie kolorow default'owych */
}

byte color::alloc(byte R,byte G,byte B)
/* Alokac*ja koloru - zwraca 0 jesli brak miejsca*/
{
int i=32;
while(i<256 && locks[i]!=0)
	 i++;
if(i<256)
	{
	MASK(R);MASK(G);MASK(B);/* Tylko n bitow jest wazne */
	handle=i;
	locks[i]++;
	nodes[handle].R=R;nodes[handle].G=G;nodes[handle].B=B;
	if(Flag)BiosSetRGB(i,nodes[handle].R,nodes[handle].G,nodes[handle].B);
	return handle;
	}
handle=0;
return 0;
}

byte color::getnear(byte R,byte G,byte B)
/* Ustalenie koloru o najblizszym RGB */
{
int i,mini=255;
int d,mind=0x0fff;
MASK(R);MASK(G);MASK(B);/* Tylko n bitow jest wazne */
for(i=1;i<256;i++)
	{
	d=ColorDist(i,R,G,B);
	if(d<mind)
		{ mind=d; mini=i; }
	}
locks[mini]++;
handle=mini;
return mini;
}

byte color::allocnear(byte R,byte G,byte B)
/* Alokacja koloru -przy braku miejsca zwraca najblizszy wg RGB */
{
int i=0,mini=255;
int d=0,mind=MAXINT;
int fr=0;
MASK(R);MASK(G);MASK(B);/* Tylko n bitow jest wazne */
for(i=1;i<256;i++)
	{
	if(locks[i]==0) fr=i;/* jEST JESZCZE MIEJSCE ! */
	d=ColorDist(i,R,G,B);
	if(d<mind)
		{ mind=d; mini=i; }
	if(mind<=1)
		goto BREAK;/* Znaleziono dokladnie taki !*/
	}
if(fr!=0)/* alokacja */
	{
	locks[fr]++;
	handle=fr;
	nodes[handle].R=R;nodes[handle].G=G;nodes[handle].B=B;
	if(Flag) BiosSetRGB(fr,R,G,B);
	return fr;/* zwrocono nowo zaalokowany */
	}
BREAK:/* Dla najpodobniejszego */
locks[mini]++;handle=mini;
return mini;/* zwrocony najpodobniejszy kolor */
}

void color::cube(int n)
/* Wpisanie na koncu tablicy RBG kostki kolorow */
{
word i,j,k,l,skok=0xffff;
MASK(skok);
skok/=n;
l=255;
cubea=n;
cubes=skok;
for(i=1;i<=n;i++)
	for(j=1;j<=n;j++)
		for(k=1;k<=n;k++)
		{
		nodes[l].G=skok*i;
		nodes[l].R=skok*j;
		nodes[l].B=skok*k;
		locks[l]=1;
		if(--l==0) goto END;
		}
END:
if(Flag) tohardware();
}

byte color::getcube(byte R,byte G,byte B)
/* Alokacja wg kostki kolorow */
{
MASK(R);MASK(G);MASK(B);/* Tylko n bitow jest wazne */
byte c2=cubes>>1;
if(R<=c2 || G<=c2 || B<=c2)
	return getnear(R,G,B);
G+=c2;R+=c2;B+=c2;
G/=cubes;R/=cubes;B/=cubes;
G--;R--;B--;
handle=255-(G*cubea*cubea+R*cubea+B);
return handle;
}

