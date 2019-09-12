
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <string.h>
#include "basic.hpp"
#include "basstrin.hpp"

// Macro  _BSTRING_C_INDEX_  forces indexing from 0 in bstring function parameters,
// else this functions are full basic compatible
// #define _BSTRING_C_INDEX_
#define DE_PRINTF printf

bstring 	left(bstring&  wz,size_t l)
{
long newlen=(l>wz.len?wz.len:l);
bstring pom(newlen+1);
if(pom.text!=NULL)
	{
	memcpy(pom.text,wz.text,newlen);
	pom.text[newlen]='\0';
	pom.len=newlen;
//DE_PRINTF("left '%s'\n",pom.text);
	}
	else onerror(OutMem);
return pom;
}

bstring 	right(bstring& wz,size_t   r)
{
long newlen=r;
bstring pom(newlen+1);
if(pom.text!=NULL)
	{
	memcpy(pom.text,wz.text+(wz.len-r),r);
	pom.len=newlen;
	pom.text[newlen]='\0';
//DE_PRINTF("right '%s'\n",pom.text);
	}
	else onerror(OutMem);
return pom;
}

void bstring::mid(bstring& expresion, size_t start,size_t lenght)
//replace a portion a string variable with string expresion
{
if(lenght==0xffff)
	lenght=expresion.len;/* Jesli wywolanie 2 parametrowe */
#ifndef _BSTRING_C_INDEX_
	start--;	//Indexing in BASIC 1..n
#endif
long newlen=(long)start+lenght;
if(newlen<len)
	newlen=len;
char* newtext=realloc(text,newlen+1,stat);
if(newtext!=NULL)
	{
	text=newtext;stat=0;
	memcpy(text+start,expresion.text,lenght);
	if(newlen>len)
		text[newlen]='\0';
	len=newlen;
	}
	else onerror(OutMem);
//DE_PRINTF("'set mid %s'\n",text);
HTEST;
}

bstring 	mid(bstring&  wz,size_t start,size_t length)
//returns a substring of a string - non swich to 0 indexing.
{
if(length==0xffff) length=wz.len;/* Jesli wywolanie 2 parametrowe */
long newlen=(long)(length<=wz.len-start+1?length:wz.len-start+1);
  bstring pom(newlen+1);
  if(pom.text!=NULL)
	{
	if(start<1 || start>wz.len || length<1){*pom.text='\0';
						goto BAD_PARAM;}
	memcpy(pom.text,wz.text+(start-1),(length<=newlen?length:newlen));
	pom.text[newlen]='\0';
	pom.len=newlen;
//DE_PRINTF("'mid %s'\n",pom.text);
	}
	else onerror(OutMem);
BAD_PARAM:
return pom;
__emit__(
#include "libownsz.inc"
);
return pom;
}
