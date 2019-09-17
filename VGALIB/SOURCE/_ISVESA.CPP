#include <dos.h>
#include <biosgraf.h>
#include <vesa.hpp>

char ISVESA=0;
unsigned int PUSTA=0xFFFF;
unsigned int far* LISTATRYBOW=&PUSTA;

int far IsVesa(VESAinfo far* buf)
{
_ES=FP_SEG(buf);
_DI=FP_OFF(buf);
_AX=0x4F00;
Int10H();
unsigned pom=_AX;
if(pom==0x004f)
	{
	LISTATRYBOW=buf->ListaTr;
	ISVESA=1;
	while(*buf->NazwaProd<' ')buf->NazwaProd++;
	return 1;
	}
	else
	return 0;
}
