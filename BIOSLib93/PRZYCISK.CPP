#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <string.h>

#include "mouse.hpp"
#include "przycisk.hpp"

void pipek::clear()
{
mosX=-1;
mosY=-1;
mosClik=0;
key=0;
}

pipek::pipek()
{
x=y=xd=yd=color=ramka=keychar=0;cliksens=-1;text=NULL;
}
int pipek::DoIt()
{
if(mosClik==cliksens && mosX>x && mosX<x+xd && mosY>y && mosY<y+yd)
		       return 1;
if(Getcomm()==keychar) return 1;
   else		       return 0;
}

void pipek::set(int lx,int ly,int lxd, int lyd, int lcol , int lram)
{
x=lx;y=ly;xd=lxd;yd=lyd;color=lcol;ramka=lram;
}

void pipek::put()
{
viewporttype  viewinfo;
getviewsettings(&viewinfo);
setfillstyle(SOLID_FILL,color);
bar(x,y,x+xd-2,y+yd-2);
setlinestyle(SOLID_LINE,0,THICK_WIDTH);
setcolor(ramka);
line(x+3,y+yd,x+xd,y+yd);
line(x+xd,y+yd,x+xd,y+2);
setviewport(x,y,x+xd-2,y+yd-2,1);
	outtextxy(2,2,text);/* Text jest obcinany */
setviewport(viewinfo.left,viewinfo.top,viewinfo.right,viewinfo.bottom,viewinfo.clip);
}

void pipek::invers()
{
setfillstyle(SOLID_FILL,ramka);
bar(x,y,x+xd-2,y+yd-2);
setlinestyle(SOLID_LINE,0,THICK_WIDTH);
setcolor(color);
line(x+3,y+yd,x+xd,y+yd);
line(x+xd,y+yd,x+xd,y+2);
outtextxy(x+5,y+1,text);
}

void pipek::InstallText(const char* t,int lkey,int lsens)
{
if(text!=NULL) delete text;
text=new char[strlen(t)+1];
strcpy(text,t);
keychar=lkey;
if(lsens!=0)
	cliksens=lsens;
}

