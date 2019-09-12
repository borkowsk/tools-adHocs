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


size_t bstring::lastblank()
{
size_t i=0;
while(text[i]==' ') i++;
return i;
}

size_t bstring::lastrblank()
{
size_t llen=len;
if(llen>0) llen--;
   else return 0;
while((long)llen>=(long)0 && text[llen]==' ')
	llen--;
return llen+1;
}

bstring 	ltrim(bstring& wz)
//returns a copy of a string with leading spaces removed
{
size_t first_no_blank=wz.lastblank();
long newlen=wz.len-first_no_blank;
  bstring pom(newlen+1);
if(pom.text!=NULL)
	{
	memcpy(pom.text,wz.text+first_no_blank,wz.len-first_no_blank);
	pom.len=newlen;
//DE_PRINTF("ltrim '%s'\n",pom.text);
	}
	else onerror(OutMem);
return pom;
}


bstring 	rtrim(bstring& wz)
//returns a copy of a string with trailing spaces removed
{
long no_blank_num=wz.lastrblank();
long newlen=no_blank_num;
  bstring pom(newlen+1);
if(pom.text!=NULL)
	{
	memcpy(pom.text,wz.text,no_blank_num);
	pom.text[newlen]='\0';
	pom.len=newlen;
//DE_PRINTF("rtrim '%s'\n",pom.text);
	}
	else onerror(OutMem);
return pom;
}

