/*
Przesuniecie kursora niezalezne od innych modulow
*/
#include <stddef.h>
#include <dos.h>
#include "biostype.h"
#include "port&int.h" 

void gotoxy(unsigned int x,unsigned int y)
{
_BH=*((unsigned char far*)0x00400062); //strona aktywna
_DH=y;
_DL=x;
_AH=0x2;
geninterrupt(0x10);
}
