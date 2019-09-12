#define _ENABLE_CHAR_OPERATORS_
#define _TEST_GOSUB_STACK_
#define _GOSUB_STACK_ 10

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "basic.hpp"
#include "basstring.hpp"
#include "gosub.hpp"
#include "data.hpp"

char  _data_table[]=
" 11\0Ala ma kota\0 12\0As to ali pies\0 13\0"
"Nikt sie "
"z nim nie bawi bo szkolony jest\0" ;
char* _data_pointer=_data_table;

bstring *P;
int main()
{
int ii,i,j,k,_end1,_end2;
double db;
char table[]="               ";
char table2[102];
bstring A=table,B="",C="3.4",D="2.5555E19";
printf("%s %s %s \n",(char*)command_line(),(char*)date_s(),(char*)time_s());
getch();
db=val(C);
B=MKI(5);
A=MKL(-100000);
printf("%d %ld %g %g\n",CVI(B),CVL(A),db,val(D));
B=MKS(8.0);
A=MKD(64.0);
printf("%f %g\n",CVS(B),CVD(A));
READDATA(db);
READDATA(A);
printf("%g %s\n",db,(char*) A);
READDATA(db);
READDATA(A);
RESTORE;
printf("%g %s\n",db,(char*) A);
A=oct(128);
B=hex(128);
printf("%s %s\n",(char*) A,(char*)B );
P=new bstring[6];
A.lset(C);

for(j=1,_end1=3;j<=_end1;j++)
    for(k=1,_end2=5;k<=_end2;k++)
	{
	 GOSUB(FORA);
	}
printf("P1='%s'\n",(char*)P[1]);
D="  !!"+str(3.5+1)+str(4.5+1)+"!!!  ";
C=A;
A="~~";
C="++"+(bstring)"(C)"+"++";
A+="(A)";
A+=B;

P[1]="  ~~P(1)p~~  ";
printf("'%s' in '%s' at %u\n",(char*)B,(char*)D,instr(D,B));
printf("~~ in '%s' at %u\n",(char*)P[1],instr(P[1],(bstring)"~~",5));
printf(" EMPTY in '%s' at %u\n",(char*)P[1],instr(P[1],(bstring)""));
printf(" '%s' in EMPTY at %u\n",(char*)P[1],instr((bstring)"",P[1]));

P[0]=left(P[1],3);
P[2]=right(P[1],3);
P[3]=string(20,'3');
P[4]=space(20);
P[5]=string(20,'*');
P[4].lset(A);
P[5].rset(A);
P[4].mid(C,10,3);
P[4].mid(P[5],10);
for(i=3;i<8;i++)
	P[3]=mid(P[1],5,i);
for(i=4;i<13;i++)
	P[3]=mid(P[1],i);
A=rtrim(ltrim(lcase(P[1])));
B=rtrim(ltrim(ucase(P[1])));
C=rtrim(ltrim(string(10,'+')));
D=rtrim(ltrim(space(10)));

GOSUB(FORD)
printf("=A='%s'\n",(char*)A);
printf("=B='%s'\n",(char*)B);
printf("=C='%s'\n",(char*)C);
printf("=D='%s'\n",(char*)D);
printf("P0='%s'\n",(char*)P[0]);
printf("P1='%s'\n",(char*)P[1]);
printf("P2='%s'\n",(char*)P[2]);
printf("P3='%s'\n",(char*)P[3]);
printf("P4='%s'\n",(char*)P[4]);
printf("P5='%s'\n",(char*)P[5]);
delete [6]P;
printf("<^^^^^^^>\n");
return 0;

FORA:
for(i=0;i<2;i++)
   for(ii=0;ii<2;ii++)
	P[1]="X"+P[1]+"Y";
RET_SUB

FORD:
for(i=1;i<5;i++)
	D=('A'+i)+D+('a'+i);
GOSUB(FORA);
printf("=D='%s'\n",(char*)D);
RET_SUB
return 1;
  }

void onerror(int code)
{
fprintf(stderr,"Run time error %d\n",code);
exit(code);
}

