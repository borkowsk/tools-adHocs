
void interrupt DOShandler(bp,di,si,ds,es,dx,cx,bx,ax,ip,cs,flags)
{
int dataseg=_DS;
/*if(ax==0x4301)
	{
	screen[0][79] ='X' + ATTR;
	}
	else*/
	screen[0][79] ='?' + ATTR;
_DI=di;/* Parametry wywolania dla orginalnego przerwania */
_SI=si;
_ES=es;
_DX=dx;
_CX=cx;
_BX=bx;
_FLAGS=flags;
_AX=ax;
asm push bp;
oldDOShandler();
asm pop bp;
ax=_AX;/* Wartosci powrotne z orginalnego przerwania */
flags=_FLAGS;
bx=_BX;
cx=_CX;
dx=_DX;
es=_ES;
si=_SI;
di=_DI;
_DS=dataseg;/* Przywrocenie segmentu danych */
screen[0][79] ='!' + ATTR;
}