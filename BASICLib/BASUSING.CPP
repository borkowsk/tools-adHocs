/* Programowal Ireneusz Moraczewski (ale podpisywal Wojciech Borkowski) */
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

char* using(bstring& s)
{
   static char  c[256];

   int a=0,i=0,k=0,m=0,z=-1,l=s.len;
   while(i < l)
   {
      switch (s.text[i])
      {
	 case '#':
	    if(z == -1)
	       a++;
	    else
	       z++;
	    break;
	 case '.':
	    z = 0;
	    if(i == 0) a=1;
	    if(i > 0 && s.text[i-1] != '#') a=1;
	    break;
      }
      if((s.text[i] != '#' && s.text[i] != '.') || i == l - 1)
      {
	    if(a > 0)
	    {
	       a+=(z+1);
	       if(z <= 0)
	       {
		 c[k] = '%';
		 if(a > 9)
		 {
		    m = a / 10;
		    c[k+1] = '0' + m;
		    c[k+2] = '0' +  a - m * 10;
		    k++;
		 }
		 else
		   c[k+1] = '0' + a;
		 c[k+2] = '.';
		 c[k+3] = '0';
		 c[k+4] = 'f';
		 k+=5;
	       }
	       else
	       {
		 c[k] = '%';
		 if(a > 9)
		 {
		    m = a / 10;
		    c[k+1] = '0' + m;
		    c[k+2] = '0' +  a - m * 10;
		    k++;
		 }
		 else
		   c[k+1] = '0' + a;
		 c[k+2] = '.';
		 if(z > 9)
		 {
		    m = z / 10;
		    c[k+3] = '0' + m;
		    c[k+4] = '0' +  z - m * 10;
		    k++;
		 }
		 else
		   c[k+3] = '0' + z;
		 c[k+4] = 'f';
		 k+=5;
	       }
	    }
	    if(s.text[i] == '%')
	    {
	       c[k] = '%';
	       c[k+1] = '%';
	       k+=2;
	    }
	    else
	    if(s.text[i] =='\\')
	    {
	       c[k] = '\\';
	       c[k+1] = '\\';
	       k+=2;
	    }
	    else
	    if(s.text[i] != '#')
	    {
	       c[k] = s.text[i];
	       k++;
	    }
	    z=-1;
	    a=0;
      }
      i++;
   }
   c[k] = '\0';
   return c;
}
