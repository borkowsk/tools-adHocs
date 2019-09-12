#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <string.h>
#include <conio.h>
#include "basic.hpp"
#include "basstrin.hpp"

// Macro  _BSTRING_C_INDEX_  forces indexing from 0 in bstring function parameters,
// else this functions are full basic compatible
// #define _BSTRING_C_INDEX_
#define DE_PRINTF printf

void bye(void)
{
int x=wherex();
int y=wherey();
_setcursortype(_NORMALCURSOR);
highvideo();
if(getenv("BAS2CTEST")!=NULL)
	{
	gotoxy(10,1);
	printf("BAS2C v 1.0 Library compiled { %s %s }", __DATE__,__TIME__);
	gotoxy(x,y);
	}
lowvideo();
}

#pragma exit bye 64

void bstring::clear()		//kasownik
	{if(!stat){
//              DE_PRINTF("Clear:'%s'\n",text);
		free(text);len=0;text="";stat=1;
		}
	return;
	__emit__('B','A','S','2','C','P','P');
	}

bstring::bstring(size_t S)
	{
//DE_PRINTF("Make temporary bstring size %d\n",S);
	S+=granularity;
	S&=~granularity;
	text=(char*)malloc(S);
	temp=1;//constructor only for temporary basstring
	stat=0;
	len=0;
	}

bstring::bstring()
	{
//DE_PRINTF("Make empty bstring\n");
	text="";len=0;temp=0;stat=1;//Prevent free static "" !!!
	}

bstring::bstring(bstring& wz)
{
if(wz.temp==1 && wz.stat==0)
{
//DE_PRINTF("Shell make temporary bstring:");
text=wz.text;	wz.text="{TEMP!}";
stat=0; 	wz.stat=1;
len=wz.len;	wz.len=0;
temp=1;		wz.temp=0;
//DE_PRINTF("'%s'\n",text);
}
else
{
//DE_PRINTF("Make bstring:");
temp=0;
len=(wz.len+1);
len+=granularity;
len&=~granularity;
text=(char*)malloc(len);
if(text!=NULL)
	{
	stat=0;
	memcpy(text,wz.text,wz.len);
	len=wz.len;
	text[len]='\0';
//	DE_PRINTF("'%s'\n",text);
	}
	else onerror(OutMem);
}
}

bstring::bstring(const char* wztext)
{
//DE_PRINTF("Make bstring from char*:");
temp=0;stat=1;
text=(char*)wztext;
len=strlen(wztext);
//DE_PRINTF("'%s'\n",text);
}

bstring& bstring::operator = (bstring& wz)
{
if(wz.text==NULL)
	{
	onerror(NullUse);
	}
if(wz.temp==1 && wz.stat==0)
{
//DE_PRINTF("Shell copy temporary bstring:");
if(!stat)
	free(text);
text=wz.text;	wz.text="{TEMP!}";
stat=0;		wz.stat=1;
len=wz.len;	wz.len=0;
temp=0;		wz.temp=0;
//DE_PRINTF("'%s'\n",text);
}
else
{
//DE_PRINTF("Deap copy:");
char* newtext=realloc(text,wz.len+1,stat);
if(newtext!=NULL)
	{
	text=newtext;stat=0;
	memcpy(text,wz.text,wz.len);
	len=wz.len;
	text[len]='\0';
//	DE_PRINTF("'%s'\n",text);
	}
	else
	{
	clear();
	onerror(OutMem);
	}
}
if(text==NULL)
	{
	onerror(NullUse);
	}
return *this;
}

bstring& bstring::operator = (const char* wztext)
{
//DE_PRINTF("Copy char* :");
text=(char*)wztext;	stat=1;
strcpy(text,wztext);
len=strlen(wztext);
//DE_PRINTF("%s\n",text);
return *this;
}

void bstring::fill(size_t n,char c)
{
long newlen=(long)n;
char* newtext=realloc(text,newlen+1,stat);
if(newtext!=NULL)
	{
	text=newtext;stat=0;
	memset(text,c,n);
	text[newlen]='\0';
	len=newlen;
//DE_PRINTF("'fill %s' [%d]\n",text,len);
	}
	else	onerror(OutMem);
return ;
}

bstring& bstring::operator += (bstring& wz)
{
//DE_PRINTF("'%s'+='%s' ->",text,wz.text);
long newlen=(long)len+wz.len;
char* newtext=realloc(text,newlen+1,stat);
if(newtext!=NULL)
	{
	text=newtext;stat=0;
	memcpy(text+len,wz.text,wz.len);
	len=newlen;
	text[len]='\0';
//DE_PRINTF("'%s' [%d]\n",text,len);
	if(wz.temp) wz.clear();//Poco ma zajmowac pamiec ?
	}
	else	onerror(OutMem);
return *this;
}

bstring& bstring::operator += (const char* wztext)
{
//DE_PRINTF("'%s'+=(char*)'%s' ->",text,wztext);
long newlen=(long)len+strlen(wztext);
char* newtext=realloc(text,newlen+1,stat);
if(newtext!=NULL)
	{
	text=newtext;stat=0;
	strcpy(text+len,wztext);
	len=newlen;
//DE_PRINTF("'%s' [%d]\n",text,len);
	}
	else	onerror(OutMem);
return *this;
}

bstring bstring::operator + (bstring& wz)
{
long newlen=(long)len+wz.len;

  bstring pom(newlen+1);
///DE_PRINTF("'%s'+'%s' ->",text,wz.text);
  if(pom.text!=NULL)
	{
	memcpy(pom.text,text,len);
	memcpy(pom.text+len,wz.text,wz.len);
	pom.len=newlen;
	pom.text[pom.len]='\0';
//DE_PRINTF("'%s'\n",pom.text);
	}
	else onerror(OutMem);
return pom;
__emit__(
#include "libownsz.inc"
);
return pom;
}

bstring operator + (const char*wztext,bstring& wz)
{
size_t wzlen=(long)strlen(wztext);
long newlen=wzlen+wz.len;
bstring pom(newlen+1);
///DE_PRINTF("(char*)'%s'+'%s' ->",wztext,wz.text);
  if(pom.text!=NULL)
	{
	memcpy(pom.text,wztext,wzlen);
	memcpy(pom.text+wzlen,wz.text,wz.len);
	pom.len=newlen;
	pom.text[newlen]='\0';
//DE_PRINTF("'%s'\n",pom.text);
	}
	else onerror(OutMem);
return pom;
}

bstring bstring::operator + (const char* wztext)
{ long newlen=(long)len+strlen(wztext);
  bstring pom(newlen+1);
///DE_PRINTF("'%s'+(char*)'%s' ->",text,wztext);
  if(pom.text!=NULL)
	{
	memcpy(pom.text,text,len);
	strcpy(pom.text+len,wztext);
	pom.len=newlen;
//DE_PRINTF("'%s'\n",pom.text);
	}
	else onerror(OutMem);
return pom;
}

