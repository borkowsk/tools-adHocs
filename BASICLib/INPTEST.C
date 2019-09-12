#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/* For heap errors debuging */
#define	  _HEAP_TESTING_

#include "basic.h"	    /* Included always ! */
#include "onerror.hpp" 	    /* Included in  main file only  !!! */

#include "basstring.hpp"
#include "randfile.hpp"
bstring A;
FILE* f;
main()
{
f=fopen("test.txt","r");
if(!f)
	onerror(IOErr);
line_input(f,A);
printf("%s\n",(char*)A);
line_input(f,A);
printf("%s\n",(char*)A);
if(!feof(f))
	{
	line_input(f,A);
	printf("%s\n",(char*)A);
	}
line_input(A);
printf("%s\n",(char*)A);
line_input(A);
printf("%s\n",(char*)A);
}