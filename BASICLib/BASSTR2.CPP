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

bstring 	lcase(bstring& wz)
//return a copy of string with all A-Z letters converted to lowercase
{
long newlen=wz.len;
  bstring pom(newlen+1);
if(pom.text!=NULL)
	{
	memcpy(pom.text,wz.text,wz.len);
	pom.text[newlen]='\0';
	strlwr(pom.text);
	pom.len=newlen;
//DE_PRINTF("lcase '%s'\n",pom.text);
	}
	else onerror(OutMem);
return pom;
}


bstring 	ucase(bstring& wz)
//return a copy of string with all a-z letters converted to uppercase
{
long newlen=wz.len;
  bstring pom(newlen+1);
if(pom.text!=NULL)
	{
	memcpy(pom.text,wz.text,wz.len);
	pom.text[newlen]='\0';
	strupr(pom.text);
	pom.len=newlen;
//DE_PRINTF("ucase '%s'\n",pom.text);
	}
	else onerror(OutMem);
return pom;
}

bstring 	space(size_t len)
//returns blank string of a specific lenght
{
long newlen=len;
  bstring pom(newlen+1);
if(pom.text!=NULL)
	{
	memset(pom.text,' ',len);
	pom.text[len]='\0';
	pom.len=newlen;
//DE_PRINTF("space '%s'\n",pom.text);
	}
	else onerror(OutMem);
return pom;
}

bstring 	string(size_t len,char c)
//returns a string consisting of one repeated characters
{
long newlen=len;
  bstring pom(newlen+1);
if(pom.text!=NULL)
	{
	memset(pom.text,c,len);
	pom.text[len]='\0';
	pom.len=newlen;
//DE_PRINTF("string '%s'\n",pom.text);
	}
	else onerror(OutMem);
return pom;
}

size_t  	instr(bstring& wz,bstring& se,size_t start)
//searches for a string within another string, returned position+1
{
#ifndef _BSTRING_C_INDEX
long s=start-1;
#else
long s=start;
#endif

char* w;
if(wz.len==0) return s;
if( s>wz.len) return 0;
w=strstr(wz.text+s,se.text);
#ifndef _BSTRING_C_INDEX
if(w==NULL) return 0;
   else return w-wz.text+1;
#else
if(w==NULL) return -1;
   else return w-wz.text;
#endif
}

double 		val(bstring& wz)
{
 char  *endptr;
 return strtod(wz.text, &endptr);
}

bstring 	chr(int c)
 {
 bstring pom(2);
 if(pom.text==NULL) onerror(OutMem);
 pom.text[0]=c;
 pom.text[1]='\0';
 pom.len=1;
 return pom;
 }

bstring 	str(double d)
{
bstring pom(81);
if(pom.text!=NULL)
	{
	sprintf(pom.text," %g",d);
	pom.len=strlen(pom.text);
	pom.text=(char*)realloc(pom.text,pom.len+1);// Zawsze mozna zmniejszyc
	}
	else onerror(OutMem);
return pom;
__emit__(
#include "libowner.inc"
);
return pom;
}



extern "C"  //from conio.h
{
int kbhit(void);
int getch(void);
};

bstring 	inkey()// returns scancode from keyboard
{
bstring pom;
if(kbhit())
	{
	pom.text=realloc(pom.text,4,pom.stat);// Parity area is faster accesed
	if(pom.text==NULL) onerror(OutMem);
	pom.stat=0;
	pom.temp=1;
	if((pom.text[0]=getch())=='\0')
		{pom.text[1]=getch();pom.text[2]='\0';pom.len=2;}
		else
		{pom.text[1]='\0';pom.len=1;}
	}
return pom;// Empty bstring
}
