#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <string.h>
#include <conio.h>
#include <stdarg.h>
#include "basic.hpp"
#include "basstrin.hpp"

// Macro  _BSTRING_C_INDEX_  forces indexing from 0 in bstring function parameters,
// else this functions are full basic compatible
// #define _BSTRING_C_INDEX_
#define DE_PRINTF printf

bstring concat(size_t n,...)
{
long newlen=0;
int i;
bstring* p;
va_list L;
va_start(L,n);
for(i=1;i<=n;i++)
	{
	p=va_arg(L,bstring*);
	newlen+=p->len;
	}
va_end(L);

bstring pom(newlen+1);
if(pom.text==NULL) onerror(1);
size_t poz=0;
va_start(L,n);
for(i=1;i<=n;i++)
	{
	p=va_arg(L,bstring*);
	memcpy(pom.text+poz,p->text,p->len);
	poz+=p->len;
	}
va_end(L);
if(poz!=newlen)
	onerror(InvCall);
pom.text[newlen]='\0';
pom.len=newlen;
return pom;
}