/*
Biblioteka Workspace User Interface - ekran VESA 256k -metody dostepu
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <values.h>
#include <mem.h>
#include <dos.h>

#include "biosgraf.h"
#include "color.hpp"
#include "picture.hpp"
#include "fonts.hpp"
#include "screens.hpp"
#include "vesa.hpp"

/* Portable section */
#define OFFSET( _X_ , _Y_ )  ((LINELEN*(_Y_))+(_X_))
inline void MEMMOVE(void far* d,void far* s,unsigned len)
{
movmem(s,d,len);
}

void ScreenVESA::PlotPixel(pixsize_t x,pixsize_t y)
{
unsigned long int offset=((long)(LINELEN)*y)+x;
unsigned long int pom;
pom=(offset>>16) ;		         /* /FFFFh */
if(pom!=BANKINA)
	AssignBank(0,pom);
/* PUT PIXEL */
*( GetFrameBuferAddr()+(offset-(pom<<16))  )=CUR_COLOR;/* %FFFFh */
}

pixpointer ScreenVESA::CalcPixelAddr(pixsize_t& x,pixsize_t& y) const
{
unsigned long int offset=((long)(LINELEN)*y)+x;
unsigned long int pom;
pom=(offset>>16) ;		         /* /FFFFh */
if(pom!=BANKINA)
	AssignBank(0,pom);
pom=(offset-(pom<<16));
pixpointer pomaddr= GetFrameBuferAddr()+pom;// Calculate adres
// Return borders for use this adress
x=min(x+(0xFFFF-pom),MAXX);
//y=MAXY;
return pomaddr;     // Return adress
}

#undef OFFSET
