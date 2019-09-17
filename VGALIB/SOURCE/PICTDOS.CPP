/*
WorkSpace User Interface - procedury specyficzne dla systemu i maszyny
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <values.h>
#include <math.h>
#include <dos.h>
#include <conio.h>

#include "biosgraf.h"
//#include "color.hpp"
#include "picture.hpp"
#include "screens.hpp"
#include "fonts.hpp"

/* Hardware specific */
void palette::fromhardware() /* Odczytanie rejestrow kolorow karty */
{
BiosGetRGBBlock(S,N,(void far*)nodes);
if(S<32)
	{
	memset(locks,0,N);
	memset(locks+S,1,32-S);/* Zablokowanie kolorow default'owych */
	}
not_lock=N-(32-S);
}

void palette::tohardware()   /* Uzgodnienie tablicy kolorow karty z wirtualna */
{
BiosSetRGBBlock(S,N,(void far*)nodes);
}

void palette::tohardware(color_handle ch)  /* Uzgodnienie jednego koloru karty z paleta */
{
RGB& rgb=nodes[ch];
ch+=S;
BiosSetRGB(ch,rgb.R,rgb.G,rgb.B);
}


void translinecopy(pixpointer  p, pixpointer b,unsigned len )
{
if(len==0)
	return;
    asm{
	mov cx,len
	cld;
	push ds
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
ROB: asm{
	stosb
	loop POCZ
	}
END: asm{
	pop ds
	}
}



