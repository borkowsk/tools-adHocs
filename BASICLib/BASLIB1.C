#include <dos.h>
#include <conio.h>

float	 timer()
{
   struct time t;
   gettime (&t);
   return (float) t.ti_hour*3600 + (float) t.ti_min*60 + t.ti_sec + (float) t.ti_hund/100;
}

int 	screen(int& line, int column)
{
   char buff[1];

   gettext(column,line,column,line,buff);
   return buff[0];
}

long int cint(double _P_)
{
double fraction,integer;
fraction=modf(_P_,&integer);
if(integer==0.0) integer=fraction;
if(integer>(-0))
	{
	if(fraction>=0.500000000) return integer+1;
		else return integer;
	}else
	{
	if(fraction<=-0.500000000) return integer-1;
		else return integer;
	}
}
