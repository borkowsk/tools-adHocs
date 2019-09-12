/* Programowal Ireneusz Moraczewski */
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

bstring date_s()
{
   struct date d;
   bstring pom(10+2);

   getdate(&d);
   pom.len = 10;
   sprintf(pom.text,"%02d-%02d-%04d",d.da_mon,d.da_day,d.da_year);
   return pom;
}

bstring time_s()
{
   struct time t;
   bstring pom(8+2);

   gettime(&t);
   pom.len = 8;
   sprintf(pom.text,"%02d:%02d:%02d",t.ti_hour,t.ti_min,t.ti_sec);
   return pom;
}
