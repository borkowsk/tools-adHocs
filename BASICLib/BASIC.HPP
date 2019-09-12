/*========================================================*/
/*        BASICALY BASIC FUNCTIONS IMPLEMENTATION         */
/*========================================================*/

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
const GosOvr=4;
const InvPar=5;
const RangE=6;
const InvCall=7;
const NullUse=8;
const IOErr=105;
const WriteErr=106;
const ReadErr=107;

//ERROR HANDLER
void cdecl onerror(int code);

#ifdef _HEAP_TESTING_
#define HTEST 	if(heapcheck()<0){fprintf(stderr,"%s %d ",__FILE__,__LINE__);onerror(HpDisr);}
#else
#define HTEST
#endif


//FUNCTION & PROCEDURES

float cdecl timer();
int   cdecl screen(int& line, int column);

#define  DefaultSeg        0xFFFF
#define  POKE(_OFS_,_VAL_) poke(_def_seg_,(_OFS_),(_VAL_))
#define  PEEK(_OFS_)       peek(_def_seg_,(_OFS_))
#define  DEF_SEG(_SEG_)    _set_def_seg(_SEG_)

extern unsigned _def_seg_;

inline
void  _set_def_seg(unsigned _SEG_)
{
if(_SEG_==DefaultSeg)
	 _def_seg_=FP_SEG(&_def_seg_);
	 else
	 _def_seg_=_SEG_;
}

//MATHEMATICAL  FUNCTION
long int cdecl cint(double _P_);

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

// Basic compatible  end of file function
//-----------------------------------------
inline
int  beof(FILE* _file_ )
 {
 //if( fileno(_file_)//
 if(_file_==stdin ) return 0;
 int z=fgetc(_file_);
 if(z!=EOF){ ungetc(z,_file_);return 0;}
    else { return 1;}
 }

// Random file service routines
//------------------------------


#define _RIDENT_ 0x7777

struct RFILE
{
FILE* file;
unsigned ident;
size_t reclen;
void* buf2;
void init(FILE* par)
	{ ident=_RIDENT_;reclen=0;buf2=NULL;file=par;}
operator FILE* ()
	{ return file; }

};


FILE*	 cdecl ropen(const char* name,size_t reclen);
fpos_t	 cdecl rlocation(FILE* f);
long	 cdecl rsize(FILE* f);
int      cdecl reof(FILE* f);
long	 cdecl rseek(FILE* f,long pos);
int	 cdecl rclose(FILE* f);

void     cdecl rfield(FILE* f,size_t n,...);/* n pair of basstring& and size_t */

void     cdecl rget(FILE* f);			/* to buffer at current pos.*/
void     cdecl rget(FILE* f,long pos);		/* to buffer at setting pos.*/
void     cdecl rget(FILE* f,void* STRUCTURE,size_t STSIZE);/* to structure */

void     cdecl rput(FILE* f);			/* from bufer */
void     cdecl rput(FILE* f,long pos);		/* from buffer at setting pos.*/
void     cdecl rput(FILE* f,void* STRUCTURE,size_t STSIZE);/* from structure */

inline int reof(FILE* f)
	{ return feof(((RFILE*)f)->file);}

//TYPE DEFINITIONS FOR ARRAYS - NON STRICT FOR BASIC
typedef long   double  extended;
typedef unsigned long  ulong;
typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef unsigned long  doubleword;
typedef unsigned int   word;
typedef unsigned char  byte;

typedef enum {FALSE,TRUE} boolean;
#endif

