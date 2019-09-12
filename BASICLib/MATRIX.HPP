/********************************************************************/
/*             Generic two dimensional array class                  */
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

// token-pasting macros; ANSI requires an extra level of indirection
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
#define _g_array_ _paste(_type_,_matrix)
#endif

#if __BCPLUSPLUS__< 0x300
// Array size for delete requiared  for versions older than 3.00
#define _DELSIZE_(__SSS__)  [__SSS__]
#else
#define _DELSIZE_(__SSS__)
#endif

class _g_array_
{
_type_** 	p;	// Pointer to vector of pointers
#if defined(_LARGE_MATRIX_) || defined(__LARGE__) || defined(__HUGE__) || defined(__COMPACT__)
unsigned 	size1;	// Max size 0xFFFF
unsigned 	size2; 	// Max size 0xFFFF
unsigned 	zero:14;// force word aligment
unsigned 	statV:1;// Static vector of pointers
unsigned 	statC:1;// Static pointed contens
#else
unsigned 	size1:15;// Max size of pointers vector is 0x7FFF
unsigned 	statV:1; // Static pointers vector
unsigned 	size2:15;// Max size of each value vector is 0x7FFF
unsigned 	statC:1; // Static contens vectors
#endif
void     _erase_static_();	// cleaning static table
void     _erase_();		// free array on heap
public:
	 _g_array_()		// constructor
		{ p=NULL;size1=size2=0;statV=statC=1;}
	 _g_array_(void* pp,size_t siz,size_t n); // constructor for aliasing static table
	~_g_array_()		// destructor
		{ _erase_();}
size_t   lbound(int )
		{ return _INDEX_MODE_;}
size_t   ubound(int d)
		{ if(d==1) return size1-1+_INDEX_MODE_;
		     else  return size2-1+_INDEX_MODE_;}
operator _type_** ()
		{ return p;}
void	 dim(size_t s,size_t z);	// dynamic alocation
void	 erase()			// dealocation
		{ if(statC) _erase_static_();else _erase_();}
void     redim(size_t s,size_t z)
		{ erase();dim(s,z);}
_type_*  operator []  (size_t i)	// line indexing operator
		{ return p[i];}
_type_&  operator ()  (size_t i,size_t j);// range check indexing operator
};

#ifdef _MATRIX_DEFINITION_
/* Prevent multiple inclusion */
_type_& _g_array_::operator ()  (size_t i,size_t j)
// range check indexing operator - inline expansion cause compile error (!?)
#if _INDEX_MODE_==0
{
if(i>=size1 || j>=size2)
		onerror(RangE); // Check bounds  AND exit
return p[i][j];
}
#else
{
if(i>=size1+_INDEX_MODE_ || i<_INDEX_MODE_ ||
     j>=size2+_INDEX_MODE_ || j<_INDEX_MODE_ )
	     onerror(RangE);	// Check bounds  AND exit
return p[i][j];
}
#endif


_g_array_::_g_array_(void* pp,size_t siz,size_t n)// constructor for aliasing static table
{
size1=siz/sizeof(_type_)/n;statC=1;
if((p=new _type_*[size1])==NULL) onerror(OutMem);// Allocation vector of pointers
	else { statV=0;
	       for(int i=0;i<size1;i++)    	 // Set pointers to vectors of items
		   p[i]=(_type_*)pp+i*n;
	     }
/* Here size field are only for range checking */
#if _INDEX_MODE_!=0
size1--;
#endif
size2=n-_INDEX_MODE_;
}

void	 _g_array_::dim(size_t s,size_t z)	// dynamic alocation
{
#if _INDEX_MODE_==0
s++;z++;					// Indexing from 0 to s and from 0 to z
#endif
if((p=new _type_*[s])==NULL) onerror(OutMem);	// Allocation vector of pointers
	else { size1=s;statV=0;		     	// If OK...
	       for(int i=0;i<size1;i++)      	// Allocation vector of items
		   if((p[i]=new _type_[z] )==NULL)
			onerror(OutMem);
#if _INDEX_MODE_!=0
			else p[i]-=_INDEX_MODE_;
	       p-=_INDEX_MODE_;
#endif
	       size2=z;statC=0;}
#ifdef _INITIALIZE_
_erase_static_();
#endif
}

void 	_g_array_::_erase_static_()	// clearing
{
if(p!=NULL)
  {
#if defined(_INITIALIZE_) || defined(_CLASS_ITEMS_)
  size_t i,j,ui=ubound(1),uj=ubound(2);
  for(i=lbound(1);i<=ui;i++)
    for(j=lbound(2);j<=uj;j++)
#  ifndef _CLASS_ITEMS_
	p[i][j]=_INITIALIZE_;
#  else
    p[i][j].erase();
#  endif
#endif
  }
}

void _g_array_::_erase_()	// dealocation
{
#if _INDEX_MODE_!=0
if( !statV && !statC )		// only if contens is on heap and vector is not empty
	p+=_INDEX_MODE_;
#endif
if( !statC ) { for(int i=0;i<size1;i++)
		 {
#if _INDEX_MODE_!=0
		 p[i]+=_INDEX_MODE_;
#endif
		 delete _DELSIZE_(size2) p[i];
		 }
	       size2=0;statC=1;
	      }
if( !statV ){ delete p;p=NULL;	// pointer to pointer vector - no destructors
	      size1=0;statV=1;}
HTEST;
}

#endif
#undef _INITIALIZE_
#undef _TYPE_
#undef _CLASS_ITEMS_
#undef _type_
#undef _g_array_
#undef _DELSIZE_
