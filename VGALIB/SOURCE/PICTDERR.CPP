/*
WUI library - default error procedure
*/
#include <stdio.h>
#include <stdlib.h>

/*
void error(char* p,int c)
{
fprintf(stderr,"%s\n",p);
if(c!=0) exit(c);
}
*/
#include <iostream.h>
void error(char* m,int a)
{
if(a>0)
       cerr<<"Error:";
   else
       cerr<<"Warning:";
cerr<<m<<endl;
if(a>0) exit(a);
}
