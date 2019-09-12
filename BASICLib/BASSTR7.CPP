/* Implementation of char related metods */
/*---------------------------------------*/
#define _ENABLE_CHAR_OPERATORS_

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

bstring operator + (const char c,bstring& wz)
{
long newlen=1+wz.len;
  bstring pom(newlen+1);
///DE_PRINTF("(char*)'%s'+'%s' ->",wztext,wz.text);
  if(pom.text!=NULL)
	{
	pom.text[0]=c;
	memcpy(pom.text+1,wz.text,wz.len);
	pom.len=newlen;
	pom.text[pom.len]='\0';
//DE_PRINTF("'%s'\n",pom.text);
	}
	else onerror(OutMem);
return pom;
__emit__(
#include "libownsz.inc"
);
return pom;
}

bstring& bstring::operator += (const char zn)
{
//DE_PRINTF("'%s'+=(char*)'%s' ->",text,wztext);
long newlen=(long)len+1;
char* newtext=realloc(text,newlen+1,stat);
if(newtext!=NULL)
	{
	text=newtext;stat=0;
	*(text+len)=zn;
	*(text+newlen)='\0';
	len=newlen;
	}
	else	onerror(OutMem);
return *this;
__emit__(
#include "libowner.inc"
);
return *this;
}

bstring& bstring::operator = (const char zn)
{
//DE_PRINTF("'%s'+=(char*)'%s' ->",text,wztext);
long newlen=1;
char* newtext=realloc(text,newlen+1,stat);
if(newtext!=NULL)
	{
	text=newtext;stat=0;
	*text=zn;
	*(text+1)='\0';
	len=newlen;
	}
	else	onerror(OutMem);
return *this;
}


bstring	   bstring::operator + (const char  c)
{
  long newlen=(long)len+1;
  bstring pom(newlen+1);
//DE_PRINTF("'%s'+(char*)'%s' ->",text,wztext);
  if(pom.text!=NULL)
	{
	memcpy(pom.text,text,len);
	pom.text[len]=c;
	pom.text[newlen]='\0';
	pom.len=newlen;
//DE_PRINTF("'%s'\n",pom.text);
	}
	else onerror(OutMem);
return pom;
}