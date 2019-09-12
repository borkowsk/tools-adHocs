#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <string.h>
#include "basic.hpp"
#include "basstrin.hpp"

void files(bstring& b)
{
b=" dir /w "+b;
system((char*)b);
}

bstring         command_line()
{//return entire command line
int i=1;
char far* cp = (char far*)MK_FP(_psp, 0x80);
while(cp[i]==' ') i++;
bstring pom(*cp-i+2);
pom.len = *cp-i+1;
_fmemcpy(pom.text,cp+i,pom.len);
strupr(pom.text);
pom.text[pom.len]='\0';
return pom;
}
