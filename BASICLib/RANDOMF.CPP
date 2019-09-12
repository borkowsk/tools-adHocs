#include <stdio.h>
#include <stdlib.h>
#include "basic.h"
#include "randfile.hpp"
#include "basstring.hpp"
#include "gosub.h"


main()
{
long l;

char a[15],c[12]="-abcdefghi-";
bstring A,C;
RFILE* R=ropen("TEST2.RND",sizeof(c));
printf("RANDOM FILE TEST\n");
rfield(R,2,&A,6,&C,6);
if(R==NULL)
	{perror("File open error");return 1;}
rput(R,c,sizeof(c));
rput(R,c,sizeof(c));
if(rseek(R,2)!=2)
	{perror("Seek error");return 1;}
rput(R,2);
rput(R);
if(rseek(R,1)!=1)
	{perror("Seek error");return 1;}
l=rlocation(R);
rget(R,a,12);
GOSUB(1,SUBR);
printf("%s at pos %ld\nFile size %ld\n",a,l,rsize(R));
rclose(R);
return 0;
SUBR:
rget(R,1);
printf("%0.*s %0.*s\n",len(A),(char*)A,len(C),(char*)C);
rget(R);
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