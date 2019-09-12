/*
Obsluga przrwania klawiatury oparta na biosie
*/
#include <stddef.h>
#include <dos.h>
#include "biostype.h"
#include "port&int.h" 
		      
int bioskey(unsigned int cmd)
{
if(cmd==1){
	_AH=0x01;
	geninterrupt(0x16);
	unsigned int Flags=_FLAGS;
	if(Flags & 0x0040)
			return 0;
	return 1;
	}
	else
	{
	_AH=cmd;
	geninterrupt(0x16);
	unsigned int ax=_AX;
	return ax;
	}
}

