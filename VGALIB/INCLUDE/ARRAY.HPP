/********************************************************************/
/*             Generic one dimensional array class                  */
/*==================================================================*/
/*      You must define type of items by define macro  _TYPE_       */
/*                 and include this file.                           */
/*        You can do it this for each type in your program	    */
/*        bat cant use macro _TYPE_ for another job.		    */
/********************************************************************/
/*
ASSUME THAT PREVIOUS INCLUDED:
	#include <stdio.h>
	#include <stdlib.h>
	#include "basic.h"
*/

//	 	   token-pasting macros from generic.h ;
// 		ANSI requires an extra level of indirection
/********************************************************************/
#define _paste( z , y )       _paste_x( z , y )
#define _paste_x( z , y )     z##y


// Indexing mode - index start from this
#ifndef _INDEX_MODE_
#define _INDEX_MODE_ 0
#endif

#ifndef _TYPE_
#error  You must define table item type .
#else
#define _type_  _TYPE_
#define _g_array_ _paste(_type_,_array)
#endif


class _g_array_
{
_type_* p;
#if  defined(_LARGE_MATRIX_) || defined(__LARGE__) || defined(__HUGE__) || defined(__COMPACT__)
unsigned 	size;  		// max size 0xFFFF
unsigned 	zero:15;	// force word aligment
unsigned 	stat:1;         // static flag
#else
unsigned 	size:15;	// max size 0x7FFF
unsigned 	stat:1;		// static flag
#endif
void	_erase_static_();	// cleaning static table */
void	_erase_()	 	// free array on heap */
		{ p+=_INDEX_MODE_;
		  delete [size]p;
		  p=NULL;
		  size=0;stat=1;}
public:
	_g_array_()		// constructor
		{ p=NULL;size=0;stat=1;}
	_g_array_(_type_* pp,size_t siz)// constructor for aliasing static table */
		{ p=pp;size=siz/sizeof(_type_);stat=1;}
       ~_g_array_()		// destructor
		{ if(!stat)_erase_();else{ p=NULL;size=0;}}
size_t	lbound()
		{ return _INDEX_MODE_;}
size_t  ubound()
		{ return size-1+_INDEX_MODE_;}
size_t	lbound(int)
		{ return _INDEX_MODE_;}
size_t  ubound(int)
		{ return size-1+_INDEX_MODE_;}
operator _type_* ()		// to pointer conversion
		{ return p;}
void	dim(size_t s); 	      	// on heap alocation
void	erase()			// dealocation or cleaning
		{ if(!stat) _erase_();else _erase_static_();HTEST;}
void    redim(size_t s)		// resizing - valid only for heap-arrays
		{ if(!stat)_erase_();dim(s);}
_type_& operator []  (size_t i)	// indexing operator
		{ return p[i];}
_type_& operator () (size_t i);	// range check indexing operator

};

inline
void _g_array_::dim(size_t s) 	      	// on heap alocation
{
#if _INDEX_MODE_==0
s++;		// Indexing from 0 to s
#endif
if((p=new _type_[s])==NULL)
		onerror(OutMem);
		else { size=s;stat=0;
		       p-=_INDEX_MODE_;}
#ifdef _INITIALIZE_
_erase_static_();
#endif
}

#ifdef _MATRIX_DEFINITION_

_type_& _g_array_::operator () (size_t i)
//range check indexing operator - inline expansion cause compile error (!?)
#if _INDEX_MODE_==0
{ if(i>=size) { onerror(RangE);return p[0]; }
	else  return p[i];}
#else
{ if(i>=size+_INDEX_MODE_ || i<_INDEX_MODE_)//Check bounds
	onerror(RangE);
	else return p[i];}
#endif

void    _g_array_::_erase_static_()	// for basic compatybility only
{
#if defined(_INITIALIZE_) || defined(_CLASS_ITEMS_)
size_t i,u=ubound();
if(p!=NULL)
	for(i=lbound();i<=u;i++)
#  ifndef _CLASS_ITEMS_
		p[i]=_INITIALIZE_;
#  else
		p[i].erase(); 		//This type must have a erase metod
#  endif

#endif
}

#endif

#undef _INITIALIZE_
#undef _CLASS_ITEMS_
#undef _TYPE_
#undef _type_
#undef _g_array_

