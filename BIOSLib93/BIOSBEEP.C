/*
DOS-owy brzeczyk niezalezny od maszyny 
Brzeczyk oparty na portach - zalezny od szybkosci procesora
*/
#include <stddef.h>
#include <dos.h>
#include "biostype.h"
#include "port&int.h" 

void dosbeep()
{
asm {
mov AH,2;
mov DL,7;
int 0x21; }
}

void portbeep(unsigned int count)
{
int i,j;
char orgbits,bits;
bits=inportb(0x61);
orgbits=bits;
for(j=0;j<=count;j++)
	{
	outportb(0x61,bits & 0xfc);
	for(i=0;i<=100;i++);
		;
	outportb(0x61, bits | 2);
	for(i=0;i<=100;i++);
		;
	}
outportb(0x61,orgbits);
}

