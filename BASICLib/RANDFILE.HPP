/* Random acces file rutines */
/*****************************/
#ifndef __RANDFILE_H__
#define __RANDFILE_H__ 1

#include <stdio.h>

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


FILE*	ropen(const char* name,size_t reclen);
fpos_t	rlocation(FILE* f);
long	rsize(FILE* f);
int     reof(FILE* f);
long	rseek(FILE* f,long pos);
int	rclose(FILE* f);

void    rfield(FILE* f,size_t n,...);/* n pair of basstring& and size_t */

void    rget(FILE* f);			/* to buffer at current pos.*/
void    rget(FILE* f,long pos);		/* to buffer at setting pos.*/
void    rget(FILE* f,void* STRUCTURE,size_t STSIZE);/* to structure */

void    rput(FILE* f);			/* from bufer */
void    rput(FILE* f,long pos);		/* from buffer at setting pos.*/
void    rput(FILE* f,void* STRUCTURE,size_t STSIZE);/* from structure */

inline int reof(FILE* f)
	{ return feof(((RFILE*)f)->file);}
#endif