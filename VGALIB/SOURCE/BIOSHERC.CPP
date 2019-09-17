/*
BIOS-owa obsluga kart graficznych - Uzupelnienie niskopoziomowe dla karty Hercules
*/
#include <conio.h>
#include <string.h>
#include <dos.h>
#include "biosgraf.h"
/*
void line2herc(int y,unsigned char far* source)
{
struct {unsigned _1:2;
	unsigned _2:2;
	unsigned _3:2;
	unsigned _4:2;
	} b;
static unsigned int offsets[]={0x0000,0x2000,0x4000,0x6000};
unsigned char far* sou=source;
unsigned char far* L1;
unsigned char far* L2;
unsigned char parity=(y%2?2:0),a;
int i;
if(y<0 || y>174) return; // Poza ekranem
y/=2;
L1=(unsigned char far*)MK_FP(0xB000,offsets[0+parity]+y*90);
L2=(unsigned char far*)MK_FP(0xB000,offsets[1+parity]+y*90);
i=0;
do{
a=*sou++;
b._4=a;
a=*sou++;
b._3=a;
a=*sou++;
b._2=a;
a=*sou++;
b._1=a;
asm MOV AL, b;
asm MOV a,AL;
*L1=a;
L1++;
i+=4;
}while(i<360);
sou=source;
i=0;
do{
a=*sou++;
b._4=a>>2;
a=*sou++;
b._3=a>>2;
a=*sou++;
b._2=a>>2;
a=*sou++;
b._1=a>>2;
asm MOV AX,b;
asm MOV a,AL;
*L2=a;
L2++;
i+=4;
}while(i<360);
}
*/

int hercgraph()
/* inicjalizacja trybu graficznego karty HGC */
{
char porty[]={0x35,0x2d,0x2e,0x07,0x5b,0x02,0x57,0x57,0x02,0x03,0,0};
int i=0;
outportb(0x03bf,0x03);
outportb(0x03b8,0x02);
for(i=0;i<0xC;i++)
	{
	outportb(0x03b4,i);
	outportb(0x03b5,porty[i]);
	}
SCREEN=(unsigned char far*)MK_FP(0xB000,0x0000);
_fmemset(SCREEN,0x00,0x8000);
i=0x2|0x0A;
outportb(0x03b8,i);
pokeb(0x0040,0x0065,i);
return 0;
}


