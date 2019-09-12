/*
  Random acces file rutines - implementation file 1
******************************************************
Positioned from 1 (not 0) for basic compatibility
******************************************************
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

FILE*	ropen(const char* name,size_t reclen)
{
RFILE* f;
FILE* ff;

ff=fopen(name,"a");
if(ff==NULL)
	{ return NULL;}
fclose(ff);

ff=fopen(name,"r+b");
if(ff==NULL)
	{ return NULL;}

f=(RFILE*) calloc(1,sizeof(RFILE));
if(f==NULL)
	{ fclose(ff);return NULL;}
f->init(ff);
f->reclen=reclen;
if(errno)
	onerror(IOErr);
rewind(F);
return (FILE*) f;
}

int	rclose(FILE* f)
{
if(((RFILE*)f)->ident!=_RIDENT_) onerror(InvCall);
int c=fclose(((RFILE*)f)->file);
if(c!=0)
	onerror(IOErr);
if(((RFILE*)f)->buf2!=NULL)
	memset(((RFILE*)f)->buf2,0,((RFILE*)f)->reclen); //Bufer is posible on use ! - must clear as Basic
//	free(((RFILE*)f)->buf2);
free(f);
return c;
}



long   rseek(FILE* f,long pos)
/* change record and return its number */
{
if(((RFILE*)f)->ident!=_RIDENT_) onerror(InvCall);
long t;
if(pos>0) pos--;
fseek(F,pos*((RFILE*)f)->reclen,SEEK_SET);
t=ftell(F);
t/=((RFILE*)f)->reclen;
return t+1;
}


fpos_t	rlocation(FILE* f)
{
if(((RFILE*)f)->ident!=_RIDENT_) onerror(InvCall);
FILE *stream=F;
fpos_t filepos;
/* report the file pointer position */
if(fgetpos(stream, &filepos)!=0)
		{
		onerror(IOErr);
		}
if(filepos%((RFILE*)f)->reclen!=0)
		onerror(IOErr);
return (filepos/((RFILE*)f)->reclen)+1;
}


long	rsize(FILE* f)
{
if(((RFILE*)f)->ident!=_RIDENT_) onerror(InvCall);
long t,s=ftell(F);
fseek(F,0,SEEK_END);
t=ftell(F);
t/=((RFILE*)f)->reclen;
fseek(F,s,SEEK_SET);
return t;
__emit__(
#include "libownsz.inc"
);
return t;
}

void    rfield(FILE* f,size_t n,...)
/* n pair of basstring& and size_t */
{
if(((RFILE*)f)->ident!=_RIDENT_) onerror(InvCall);
bstring* p=NULL;
size_t i;
int l,poz;
va_list L;
if(((RFILE*)f)->buf2==NULL)
	if((((RFILE*)f)->buf2=calloc(((RFILE*)f)->reclen,1))==NULL)
			onerror(OutMem);
va_start(L,n);
poz=0;
for(i=0;i<n;i++)
	{
	if(poz>=((RFILE*)f)->reclen)
			onerror(InvPar);
	p=va_arg(L,bstring*);
	l=va_arg(L,int);
	p->clear();
	p->text=(char*)(((RFILE*)f)->buf2)+poz;
	p->len=l;
	p->stat=1;
	poz+=l;
	}
va_end(L);
}

void    rget(FILE* f)
/* to buffer at current pos.*/
{
if(((RFILE*)f)->ident!=_RIDENT_) onerror(InvCall);
if(((RFILE*)f)->buf2!=NULL)
	{
	if(fread(((RFILE*)f)->buf2, ((RFILE*)f)->reclen , 1, F )!=1)
			memset(((RFILE*)f)->buf2,0,((RFILE*)f)->reclen);//Basic compatibility
//			onerror(ReadErr);
	}
	else onerror(InvCall);
}

void    rput(FILE* f)
/* from bufer */
{
if(((RFILE*)f)->ident!=_RIDENT_) onerror(InvCall);
if(((RFILE*)f)->buf2!=NULL)
	{
	if(fwrite(((RFILE*)f)->buf2, ((RFILE*)f)->reclen , 1, F )!=1)
			onerror(WriteErr);
	}
	else onerror(InvCall);
}

void    rget(FILE* f,long pos)
/* to buffer at setting pos.*/
{
if(((RFILE*)f)->ident!=_RIDENT_) onerror(InvCall);
if(pos>0) pos--;
errno=0;
if(((RFILE*)f)->buf2!=NULL)
	{
	if( fseek(F,pos*((RFILE*)f)->reclen,SEEK_SET)!=0)
		if(errno){onerror(IOErr);}
			else
			{
			errno=ERANGE;
			onerror(IOErr);
			}
	if(fread(((RFILE*)f)->buf2, ((RFILE*)f)->reclen , 1, F )!=1)
			memset(((RFILE*)f)->buf2,0,((RFILE*)f)->reclen);//Basic compatibility
//			onerror(ReadErr);
	}
	else onerror(InvCall);
}


void    rput(FILE* f,long pos)
/* from buffer at setting pos.*/
{
if(((RFILE*)f)->ident!=_RIDENT_) onerror(InvCall);
if(pos>0) pos--;
if(((RFILE*)f)->buf2!=NULL)
	{
	if( fseek(F,pos*((RFILE*)f)->reclen,SEEK_SET)!=0)
			{
			errno=ERANGE;
			onerror(IOErr);
			}
	if(fwrite(((RFILE*)f)->buf2, ((RFILE*)f)->reclen , 1, F )!=1)
			onerror(WriteErr);
	}
	else onerror(InvCall);
}


