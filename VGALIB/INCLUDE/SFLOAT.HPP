#ifndef _SFLOAT_HPP_
#define _SFLOAT_HPP_   1
#include <math.h>
extern void error(char*,int);

#ifdef __BORLANDC__
#define OFLOW 0x8
#endif

template<class baseint>
class sfloat
{
static const baseint MAXVALUE;
baseint val;
static void overflow()
       {
       if(_FLAGS&OFLOW)
                       error("Short Float overflow !\n",1);
       }
public:
static baseint MAX()
       { return MAXVALUE; }

sfloat(double rval)
{
if(fabs(rval)<=1)
	val=rval*MAX();
}

sfloat(sfloat& sf)
{
val=sf.val;
}

sfloat& operator = (sfloat& sf)
{
val=sf.val;
return *this;
}

double expand()
{
return (double)val/(double)MAX();
}

operator double()
{
return expand();
}

sfloat  operator + (sfloat sf)
{
sf.val+=val;
overflow();
return sf;
}

sfloat  operator - (sfloat sf)
{
sf.val-=val;
overflow();
return sf;
}

sfloat  operator / (sfloat sf)
{
//  b     c     b   M
//  -  /  -  =  - * -
//  M     M     M   c
if(val>sf.val) error("Sfloat divide error",1);
sf.val=(val*(long)MAX())/sf.val;
return sf;
}

sfloat  operator * (sfloat sf)
{
sf.val=(long)val*(long)sf.val/(long)MAX();
return sf;
}

};
/*
#include <limits.h>
const sfloat<int>::MAXVALUE=INT_MAX;
const sfloat<long>::MAXVALUE=LONG_MAX;
const sfloat<short>::MAXVALUE=SHRT_MAX;
const sfloat<signed char>::MAXVALUE=SCHAR_MAX;
*/
#endif

