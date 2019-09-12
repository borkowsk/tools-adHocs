#include <dos.h>
#include <math.h>
#include <conio.h>

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
