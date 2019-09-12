//#define _MASM_INLINE_
//#define _GOSUB_BY_STACK_

#define  _ENABLE_CHAR_OPERATORS_
#define  _HEAP_TESTING_
#define _MATRIX_DEFINITION_ /*  Non inline matrix metod
				included only in this file */

#include <stdio.h>
#include <stdlib.h>
#include "basic.hpp"
#include "basstring.hpp"
#include "randfile.hpp"
#include "gosub.hpp"
#include "data.hpp"
#include "onerror.hpp"

#define _TYPE_ bstring
#include "array.hpp"
bstring_array P;

char  _data_table[]=
" 11\0Ala ma kota\0 12\0As to ali pies\0 13\0"
"Nikt sie "
"z nim nie bawi bo szkolony jest\0" ;
char* _data_pointer=_data_table;

void SetABP(bstring& A,bstring& B,bstring_array& P)
{
A=oct(128);
B=hex(128);
printf("%s %s\n",(char*) A,(char*)B );
P.dim(6);

}


void ReadAdb(bstring& A,double& db)
{
READDATA(db);
READDATA(A);
printf("%g %s\n",db,(char*) A);
READDATA(db);
READDATA(A);
RESTORE;
DEF_SEG(0xB800);
POKE(99,'?');
printf("%g %s\n",db,(char*) A);
DEF_SEG(DefaultSeg);
}

void trims(bstring_array& P,bstring& A,bstring& B,bstring& C,bstring& D)
{
int i;
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
}

main()
{
int ii,i,j,k,_end1,_end2;
double db;
char table[]="               ";
char table2[102];
bstring A=table,B="",C="3.4",D="2.5555E19",E;
putchar('\n');
E='a';
E+='b';
E='a'+2;
E=2+'P';
if(E=='b')
	onerror(1001);
	else
	printf("E=%s\n",(char*) E);
E=C+'a';
E='a'+C;
printf("E=%s\n",(char*) E);
db=val(C);
B=MKI(5);
A=MKL(-100000);
printf("%d %ld %g %g\n",CVI(B),CVL(A),db,val(D));
B=MKS(8.0);
A=MKD(64.0);
printf("%f %g\n",CVS(B),CVD(A));

SetABP(A,B, P);
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

ReadAdb(A,db);

P[1]="  ~~P(1)p~~  ";
printf("'%s' in '%s' at %u\n",(char*)B,(char*)D,instr(D,B));
printf("~~ in '%s' at %u\n",(char*)P[1],instr(P[1],(bstring)"~~",5));
printf(" EMPTY in '%s' at %u\n",(char*)P[1],instr(P[1],(bstring)""));
printf(" '%s' in EMPTY at %u\n",(char*)P[1],instr((bstring)"",P[1]));

trims(P,A,B,C,D);

GOSUB(FORD);
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
P.erase();

do
;
while( inkey()=="" );

printf("<^^^^^^^>\n");
return 1;
FORA:
for(i=0;i<2;i++)
   for(ii=0;ii<2;ii++)
	P[1]="X"+P[1]+"Y";
RET_SUB

FORD:
for(i=1;i<5;i++)
//	D=(chr(asc((bstring)"A")+i))+D+(chr(asc((bstring)"a")+i));
	D=('A'+i)+D+('a'+i) ;
RET_SUB
  }


