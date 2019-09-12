/* BASIC`s->C translator functions */
/*---------------------------------*/

#ifndef __BASIC_H__
#define __BASIC_H__ 1

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <dos.h>

//BAS ERROR CODE
//--------------
const OutMem=1;
const HpDisr=2;
const RetOvr=3;
const InvPar=4;
const RangE=5;
const InvCall=6;
const NullUse=7;
const IOErr=105;
const WriteErr=106;
const ReadErr=107;

//ERROR HANDLER
void onerror(int code);

#ifdef _HEAP_TESTING_
#define HTEST 	if(heapcheck()<0){fprintf(stderr,"%s %d ",__FILE__,__LINE__);onerror(HpDisr);}
#else
#define HTEST
#endif

//TYPE DEFINITIONS FOR ARRAYS
typedef long   double  extended;
typedef unsigned long  ulong;
typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

typedef unsigned long  doubleword;
typedef unsigned int   word;
typedef unsigned char  byte;

typedef enum {FALSE,TRUE} boolean;

//MATHEMATICAL INLINE FUNCTION
inline int sgn(double i)
{
if(i==0) return 0;
   else
   if(i<0) return -1;
      else return 1;
}

inline int sgn(long int i)
{
if(i==0) return 0;
   else
   if(i<0) return -1;
      else return 1;
}

inline long int ent(double _P_)
	{return (_P_<0.0?(_P_)-1:(_P_));}

inline long int fix(double _P_)
	{return (_P_);}          //Its standard C conversion.
				 //Can change to ( _P_ )

inline long int cint(double _P_)
	{double fraction,integer;
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
#endif*