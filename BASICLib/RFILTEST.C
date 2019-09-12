#include <stdio.h>
#include <stdlib.h>
#include "basic.hpp"
#include "randfile.hpp"
#include "basstring.hpp"
#include "gosub.hpp"
#include "data.hpp"
char _data_table[]=	"-123456789-\0"
			"-abcdefghi-\0"
			"-ABCDEFGHI-\0"
			"[[[[[]]]]]]\0"
			"{{{{{}}}}}}\0";
char* _data_pointer=_data_table;
FILE* file[20];
main()
{
long l;
char a[15]="\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
char c[12]="static_data";
bstring A,C,D;
A=command_line();
input(C);
input(D);
line_input(A);
input(C);
printf("RANDOM FILE TEST %0.*s\n",len(A),(char*)A);
files((bstring)"*.RND");
file[2]=ropen("TESTQ.RND",sizeof(c));
rfield(file[2],2,&A,6,&C,6);
if(file[2]==NULL)
	{perror("File open error");return 1;}
rput(file[2],c,sizeof(c));
rput(file[2],c,sizeof(c));
if(rseek(file[2],2)!=2)
	{perror("Seek error");return 1;}
rput(file[2],2);
READDATA(D);
A.lset(D);
READDATA(D);
C.lset(D);
rput(file[2]);
if(rseek(file[2],1)!=1)
	{perror("Seek error");return 1;}
l=rlocation(file[2]);
rget(file[2],a,12);
GOSUB(SUBR);
fprintf(stderr,"%s at pos %ld\nFile size %ld\n",a,l,rsize(file[2]));
rseek(file[2],54);
l=rlocation(file[2]);
rget(file[2]);
fprintf(stderr,"%s at pos %ld\nFile size %ld\n",a,l,rsize(file[2]));
rseek(file[2],55);
l=rlocation(file[2]);
rget(file[2],a,12);
fprintf(stderr,"%s at pos %ld\nFile size %ld\n",a,l,rsize(file[2]));
rget(file[2],56);
l=rlocation(file[2]);
fprintf(stderr,"%s at pos %ld\nFile size %ld\n",a,l,rsize(file[2]));
A.lset(D);C.lset(D);
fprintf(stderr,"%s at pos %ld\nFile size %ld\n",a,l,rsize(file[2]));
rseek(file[2],144);
l=rlocation(file[2]);
rput(file[2]);
fprintf(stderr,"%s at pos %ld\nFile size %ld\n",a,l,rsize(file[2]));
rseek(file[2],145);
rput(file[2],a,12);
rseek(file[2],145);
l=rlocation(file[2]);
rget(file[2],a,12);
fprintf(stderr,"%s at pos %ld\nFile size %ld\n",a,l,rsize(file[2]));
rput(file[2],146);
rseek(file[2],146);
l=rlocation(file[2]);
rget(file[2],a,12);
fprintf(stderr,"%s at pos %ld\nFile size %ld\n",a,l,rsize(file[2]));
rget(file[2],146);
printf("{%0.*s %0.*s}\n",len(A),(char*)A,len(C),(char*)C);
rclose(file[2]);
printf("After close{%0.*s %0.*s}!\n",len(A),(char*)A,len(C),(char*)C);
return 0;

SUBR:
rget(file[2],1);
printf("%0.*s %0.*s\n",len(A),(char*)A,len(C),(char*)C);
rget(file[2]);
printf("%0.*s %0.*s\n",len(A),(char*)A,len(C),(char*)C);
RET_SUB;
}

void onerror(int code)
{
fprintf(stderr,"Run time error %d\n",code);
if(errno!=0)
	perror("OS error");
exit(code);
}