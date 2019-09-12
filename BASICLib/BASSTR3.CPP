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

bstring 	input(size_t n)// reads n character from input
{
bstring pom(n+1);
if(pom.text==NULL) onerror(OutMem);
fgets(pom.text,n,stdin);
pom.len=n;
return pom;
}

bstring 	input(size_t n,FILE* s)// reads n character from file
{
bstring pom(n+1);
if(pom.text==NULL) onerror(OutMem);
fgets(pom.text,n,s);
pom.len=n;
return pom;
}
