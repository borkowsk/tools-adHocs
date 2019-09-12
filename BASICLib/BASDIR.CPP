/* Programowal Ireneusz Moraczewski */
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <dir.h>
#include <string.h>
#include "basic.hpp"
#include "basstrin.hpp"

// Macro  _BSTRING_C_INDEX_  forces indexing from 0 in bstring function parameters,
// else this functions are full basic compatible
// #define _BSTRING_C_INDEX_
#define DE_PRINTF printf



bstring curdir(int d)
{
   bstring pom(MAXDIR+3);

   if(d) d -= 64;
   if(getcurdir(d,pom.text+3) < 0) onerror(InvCall);
   if(d == 0)
      pom.text[0] = 65+getdisk();
   else
      pom.text[0] = 64 + d;
   pom.text[1] = ':';
   pom.text[2] = '\\';
   pom.len = strlen(pom.text);
   return pom;
}
