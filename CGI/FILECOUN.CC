#include <stdio.h>
#include <errno.h>
#include "filecoun.hpp"

file_counter::file_counter(const char*name)
{
ecode=0;
errno=0;
f=fopen(name,"r+");
if(f==NULL)
      if(errno==2)
	{
	errno=0;
	f=fopen(name,"w+");// New file
		if(f==NULL)
		     goto ERROR;
		     else
		     errno=0;
	fprintf((FILE*)f,"%d ",0);
	if(fflush((FILE*)f)==EOF)
		goto ERROR;
	reset();
	}
	else goto ERROR;

//reset();
return;
ERROR:
ecode=errno;errno=0;
}

file_counter::~file_counter()
{
if(f!=NULL)
	fclose((FILE*)f);
}

void file_counter::operator ++ ()
{
errno=0;
unsigned long pom=(*this);
pom++;
reset();
fprintf((FILE*)f,"%ld ",pom);
if(fflush((FILE*)f)==EOF)
	{ecode=errno;errno=0;return;}
}

void file_counter::reset()
{
if(fseek((FILE*)f,0,SEEK_SET)==EOF)
	{ecode=errno;errno=0;return;}
}

file_counter::operator unsigned long ()
{
unsigned long pom;
reset();
if(fscanf((FILE*)f,"%ld",&pom)==EOF)
	{ecode=errno;errno=0;return -1;}
return pom;
}