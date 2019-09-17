#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <string.h>
#include "mouse.hpp"
#define HERCMONO 7

void MouseDriver::DefaultMouseRoutine()
{
}
unsigned   MouseDriver::IsMouse=0;
MouseEvent MouseDriver::GlEvt;
int        MouseDriver::hardware=0;

int MouseDriver::Init(int nhardware)
{
/* save graphics hardware available */
hardware=nhardware;
/* If Hercules then set BIOS variable */
if(hardware==HERCMONO)
	pokeb( 0 , 0x449 , 6 );
_AX=0;// inicjacja
geninterrupt(51);// run a lot of time ???
if(_AX==0 )
	IsMouse=0;
	else
	IsMouse=1;
return IsMouse;
}

void MouseDriver::ShowCursor()
{
if(!IsMouse) return;
_AX=1;
geninterrupt(51);
}

void MouseDriver::HideCursor()
{
if(!IsMouse) return;
_AX=2;
geninterrupt(51);
}


int MouseDriver::CheckStatus()
{
if(IsMouse)
	{
	_AX=3;
	geninterrupt(51);
	DefEvt.MosX=_CX;
	DefEvt.MosY=_DX;
	DefEvt.MosClick=_BX;
	return DefEvt.MosClick;
	}
        else return 0;
}

void MouseDriver::SetMouseCursorPosition(int X,int Y)
{
_CX=X;
_DX=Y;
_AX=4;
geninterrupt(51);
}

unsigned MouseDriver::GetButtonPressInformation(int button,int& X,int& Y)
{
_BX=button;
_AX=5;
geninterrupt(51);
unsigned pom=_BX;
X=_CX;
Y=_DX;
return pom;
}

unsigned MouseDriver::GetButtonReleaseInformation(int button,int& X,int& Y)
{
_BX=button;
_AX=6;
geninterrupt(51);
unsigned pom=_BX;
X=_CX;
Y=_DX;
return pom;
}

void MouseDriver::SetMinMaxHorizontalPosition(int minx,int maxx)
{
_CX=minx;
_DX=maxx;
_AX=7;
geninterrupt(51);
}

void MouseDriver::SetMinMaxVerticalPosition(int miny,int maxy)
{
_CX=miny;
_DX=maxy;
_AX=8;
geninterrupt(51);
}

void MouseDriver::SetGraphicsCursorBlock(int hotX,int hotY,void far* maskblock)
{
_BX=hotX;
_CX=hotY;
_DX=FP_OFF(maskblock);
_ES=FP_SEG(maskblock);
_AX=9;
geninterrupt(51);
}

void MouseDriver::SetTextCursor(int hardSoft,unsigned mask1,unsigned mask2)
{
_BX=hardSoft;
_CX=mask1;
_DX=mask2;
_AX=10;
geninterrupt(51);
}

void MouseDriver::ReadMouseMotionCounters(int& motX,int& motY)
{
_AX=11;
geninterrupt(51);
motX=_CX;
motY=_DX;
}

void MouseDriver::SetInterruptSubrutine(void far* address,MouseMask Mask)
{
_CX=Mask;
_DX=FP_OFF(address);
_ES=FP_SEG(address);
_AX=12;
geninterrupt(51);
}

void MouseDriver::LightPenEmulationOn()
{
_AX=13;
geninterrupt(51);
}

void MouseDriver::LightPenEmulationOff()
{
_AX=14;
geninterrupt(51);
}

void MouseDriver::SetMickeyPixelRatio(unsigned RX,unsigned RY)
{
_CX=RX;
_DX=RY;
_AX=15;
geninterrupt(51);
}

void MouseDriver::ConditionalOff(int x1,int y1,int x2,int y2)
{
_CX=x1;
_DX=y1;
_SI=x2;
_DI=y2;
_AX=16;
geninterrupt(51);
}

void MouseDriver::SetDoubleSpeedTreshould(unsigned imppersec)
{
_DX=imppersec;
_AX=19;
geninterrupt(51);
}

void far* MouseDriver::SwapInterrruptSubrutines(void far* address,MouseMask Mask,MouseMask& OldMask)
{
void far* pom=address;
_DX=FP_OFF(pom);
_ES=FP_SEG(pom);
_CX=Mask;
_AX=20;
geninterrupt(51);
Mask=_CX;
pom=MK_FP(_ES,_DX);
OldMask=Mask;
return pom;
}

void MouseDriver::AllocStateBufer()
{
unsigned size;
if(StateBuffer==NULL)
   {
   _AX=21;
   geninterrupt(51);
   size=_AX;
   StateBuffer=new char[size];
   }
}

void MouseDriver::SaveMouseDriverState()
{
_DX=FP_OFF(StateBuffer);
_ES=FP_SEG(StateBuffer);
_AX=22;
geninterrupt(51);
}

void MouseDriver::RestoreMouseDriverState()
{
_DX=FP_OFF(StateBuffer);
_ES=FP_SEG(StateBuffer);
_AX=23;
geninterrupt(51);
}

int MouseDriver::SetAlternateInterruptSubrutine(void far* address,MouseMask Mask)
{
_DX=FP_OFF(address);
_ES=FP_SEG(address);
_CX=Mask;
_AX=24;
geninterrupt(51);
if((signed)_AX==-1) return 0;
   else    return 1;
}

void far* MouseDriver::GetAlternateInterruptSubrutine(MouseMask& Mask)
{
void far* address;
unsigned pomA,pomC;
_CX=Mask;
_AX=25;
geninterrupt(51);
pomA=_AX;
pomC=_CX;
address=MK_FP(_ES,_DX);
Mask=pomC;
if((signed)pomA==-1) return NULL;
   else    return address;
}

void MouseDriver::SetMouseSensitivity(int sensX,int sensY,unsigned tres)
{
_DX=tres;
_CX=sensY;
_BX=sensX;
_AX=26;
geninterrupt(51);
}

void MouseDriver::GetMouseSensitivity(int& sensX,int& sensY,unsigned& tres)
{
_AX=27;
geninterrupt(51);
unsigned ltres=_DX;//Tymczasowe zapisanie zeby zapobiec
unsigned lsensY=_CX;//mozliwej modyfikacji rejestrow
unsigned lsensX=_BX;// ...
tres=ltres;
sensY=lsensY;
sensX=lsensX;
}

void MouseDriver::SetMouseInterruptRate(unsigned intpersec)
{
// dziala tylko z mysza InPort
_BX=intpersec;
_AX=28;
geninterrupt(51);
}

void MouseDriver::SetCRTPageNumber(unsigned page)
{
_BX=page;
_AX=29;
geninterrupt(51);
}

unsigned MouseDriver::GetCRTPageNumber()
{
_AX=30;
geninterrupt(51);
return _BX;
}

void far* MouseDriver::DisableDriver()
{
_AX=31;
geninterrupt(51);
if((signed)_AX==-1) return NULL;
      return MK_FP(_ES,_BX);
}

void MouseDriver::EnableDriver()
{
_AX=32;
geninterrupt(51);
}

int MouseDriver::SoftwareReset()
{
_AX=33;
geninterrupt(51);
if((signed)_AX==-1) return _BX;
   else return 0;
}
