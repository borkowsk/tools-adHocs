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

bstring 	hex(long int d)
{
bstring pom(81);
if(pom.text!=NULL)
	{
	sprintf(pom.text," %lx",d);
	pom.len=strlen(pom.text);
	pom.text=(char*)realloc(pom.text,pom.len+1);// Zawsze mozna zmniejszyc
	}
	else onerror(OutMem);
return pom;
}

bstring 	oct(long int d)
{
bstring pom(81);
if(pom.text!=NULL)
	{
	sprintf(pom.text," %lo",d);
	pom.len=strlen(pom.text);
	pom.text=(char*)realloc(pom.text,pom.len+1);// Zawsze mozna zmniejszyc
	}
	else onerror(OutMem);
return pom;
}
