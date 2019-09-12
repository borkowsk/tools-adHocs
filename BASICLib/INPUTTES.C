#include <stdio.h>
#include <stdlib.h>
#include "basic.hpp"
#include "randfile.hpp"
#include "basstring.hpp"
#include "gosub.hpp"
#include "data.hpp"
#include "onerror.hpp"
bstring A;

main()
{
printf("Input Echo test - Ctrl-C ->quit\n");
printf("from file:");
FILE* f=fopen("test.txt","r");
if(!f) onerror(IOErr);
line_input(f,A);
	printf("%s\n",(char*)A);
input(f,A);
	printf("%s\n",(char*)A);
input(f,A);
	printf("%s\n",(char*)A);
line_input(f,A);
	printf("%s\n",(char*)A);
if(!feof(f))
	{
	line_input(f,A);
	printf("reszta '%s'\n",(char*)A);
	}
printf("koniec pliku !\n");
fclose(f);

randomize();
do{
if(random(2)!=0)
	{
	printf("Input  A:");
	input(A);
	printf("%s\n",(char*)A);
	}
	else
	{
	printf("LInput A:");
	line_input(A);
	printf("%s\n",(char*)A);
	}
}while(1);
}