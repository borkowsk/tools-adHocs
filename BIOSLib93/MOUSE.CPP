#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <string.h>
#include "mouse.hpp"

 int Interface::key=0;
 unsigned
 int Interface::IsMouse=0;
 int Interface::mosX=-1;
 int Interface::mosY=-1;
 int Interface::mosClik=0;
 int Interface::gdriver=0;

int Interface::MousePresent()
{
 int gmode, errorcode;
  /* detect graphics hardware available */
/* detectgraph(&gdriver, &gmode);
 if(gdriver==HERCMONO)
	pokeb( 0 , 0x449 , 6 ); */
_AX=0;// inicjacja
geninterrupt(51);// run long time ???
if(_AX==0 )
	IsMouse=0;
	else
	IsMouse=1;
return IsMouse;
}

void Interface::VisCursor()
{
if(!IsMouse) return;
_AX=1;
geninterrupt(51);
}

void Interface::UnVisCursor()
{
if(!IsMouse) return;
_AX=2;
geninterrupt(51);
}

void  Interface::SetCursorPos(int X,int Y)
{
if(!IsMouse) return;
_CX=X;  _DX=Y; _AX=4;
geninterrupt(51);
}

char Interface::Getcomm()
{
if(kbhit())  return (key=getch());
      else   return  key;
}

int Interface::CheckClick()
{
if(IsMouse)
	{
	_AX=3;
	geninterrupt(51);
	mosX=_CX;
	mosY=_DX;
	mosClik=_BX;
	return mosClik;
	}
      else return 0;
}

void Interface::MouseBox(int MinX,int MinY,int MaxX,int MaxY)
{
if(!IsMouse) return;
_CX=MinX;
_DX=MaxX;
_AX=7;
geninterrupt(51);
_CX=MinY;
_DX=MaxY;
_AX=8;
geninterrupt(51);
}