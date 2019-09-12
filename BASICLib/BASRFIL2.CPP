/*
  Random acces file rutines - implementation file 2
*****************************************************
Positioned from 1 (not 0) for basic compatibility
*****************************************************
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include "basic.hpp"
#include "basstring.hpp"

#define F ( ((RFILE* )f) -> file )

void    rput(FILE* f,void* STRUCTURE,size_t STSIZE)
/* from structure */
{
if(STSIZE<((RFILE*)f)->reclen)
		onerror(InvCall);
if(fwrite( STRUCTURE , ((RFILE*)f)->reclen , 1, F )!=1)
			onerror(WriteErr);

}

void    rget(FILE* f,void* STRUCTURE,size_t STSIZE)
/* to structure */
{
if(STSIZE<((RFILE*)f)->reclen)
		onerror(InvCall);
if(fread( STRUCTURE , ((RFILE*)f)->reclen , 1, F )!=1)
			memset(STRUCTURE,0,STSIZE);//Basic compatibility
//			onerror(ReadErr);
}
