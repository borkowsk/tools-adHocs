/* Programowal Ireneusz Moraczewski */
/* Przerobil na postac proceduralna W.Borkowski */

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

void 	 input(FILE* file,bstring& pom)
//read string from file to "," ";" or "EOL"
//bstring finps(FILE *file)
{
   int z;
   int i=0,j=0,k=0,l=0,t=0;
   char* p=realloc(pom.text,256,pom.stat);
   if(p!=NULL)
	 {
	  pom.text=p;
	  pom.stat=0;
	 }
	 else onerror(1);
   for(i=0; i<255; i++)
   {
      if((z = fgetc(file)) == EOF) onerror(130);
      if(z == ',' || z == '\n')
	 break;
      else
      if(z == 32 && t > 0)
      {
	   l++;
      }
      else
      {
	if(z != 32 && l)
	 {
	    for(j=k; j<k+l; j++)
	       pom.text[j] = 32;
	    k+=l;
	    l=0;
	 }
	pom.text[k] = z;
	if(z != 32 || t == 1)
	{
	   t = 1;
	   k++;
	}
      }
   }
   pom.text[k] = NULL;
   pom.len = k;
   pom.text=realloc(pom.text,k+1,pom.stat);
}

void 	 line_input(FILE* file,bstring& pom)
// read string from file to EOL
// bstring flinps(FILE *file)
{
   int z,i;
   char* p=realloc(pom.text,256,pom.stat);
   if(p!=NULL){
	       pom.text=p;
	       pom.stat=0;
	      }
       else onerror(1);
   if((z=feof(file))!=0)
		onerror(107);
   for(i=0;i<255;i++)
	{
	z=getc(file);
        if( z=='\n'|| z==EOF)
                     break;
	pom.text[i]=z;
	}
   pom.text[i]='\0';
   pom.len = i;
//   if(i==1 && pom.text[0]<32)
//	{	z=1;	}
   pom.text=realloc(pom.text,pom.len+1,pom.stat);
}


