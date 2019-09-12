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

bstring		_mk(int i)
{
bstring pom(sizeof(int));
if(pom.text==NULL) onerror(OutMem);
*(int*)pom.text=i;
pom.len=sizeof(int);
pom.temp=0;
return pom;
}

bstring		_mk(long int i)
{
bstring pom(sizeof(long int));
if(pom.text==NULL) onerror(OutMem);
*(long int*)pom.text=i;
pom.len=sizeof(long int);
pom.temp=0;
return pom;
}

bstring		_mk(float i)
{
bstring pom(sizeof(float));
if(pom.text==NULL) onerror(OutMem);
*(float*)pom.text=i;
pom.len=sizeof(float);
pom.temp=0;
return pom;
}

bstring		_mk(double i)
{
bstring pom(sizeof(double));
if(pom.text==NULL) onerror(OutMem);
*(double*)pom.text=i;
pom.len=sizeof(double);
pom.temp=0;
return pom;
}

void bstring::lset(bstring& expresion)
//left justifies the value of a string expression in bstring
{
size_t l=(len>expresion.len?expresion.len:len);
memcpy(text,expresion.text,l);
memset(text+l,' ',len-l);
//DE_PRINTF("lset '%s'\n",text);
}

void bstring::rset(bstring& expresion)
//right justifies the value of a string expression in bstring
{
size_t l=(len>expresion.len?expresion.len:len);
memset(text,' ',(len-l));
memcpy(text+(len-l),expresion.text,l);
//DE_PRINTF("rset '%s'\n",text);
}

