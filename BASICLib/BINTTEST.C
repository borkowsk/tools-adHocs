//#pragma inline
//#define _MASM_INLINE_



#include <stdio.h>
#include <stdlib.h>
#include "basic.h"
#include "onerror.hpp"
/*--------------------------------------------------------*/
/* Default GOSUB is by its own "gosub_stack[GOSUB_STACK]" */
/* You may change len of this stack by define:            */
#define GOSUB_STACK 128
/* You may use program stack for GOSUB by define:	  */
//#define _GOSUB_BY_STACK_
/* But this metod make probably impredictable results if  */
/* you use math package or nested local variable.	  */
/*--------------------------------------------------------*/
#include "gosub.h"



double data[]={0.0,
		0.3, 0.499999,0.5,0.7, 0.999999,
	       99.3,99.5,99.7,
	       -0.3,-0.499999,-0.5,-0.7,-0.999999,
	       -99.3,-99.5,-99.7};

main()
{
double N,fraction=0,integer=0,scratch;
int i;
printf
("\n\n\n\n     N        Int(N)    Cint(N)    Fix(N)  \n");
for(i=0;i<(sizeof(data)/sizeof(double));i++)
	{
	N=data[i];
	GOSUB(1,SUBRUTINE);
//	printf("%g are %g and %g\n",N, integer, fraction);
	}
return 0;

SUBRUTINE:
	fraction=1;
	integer=N*N;
//	fraction = modf(N, &integer); //zwraca bledne wyniki - koprocesor i gosub !
	printf("%10g %10ld %10ld  %10ld %d \n",N,int(N),cint(N),fix(N),sgn(N));
RET_SUB;
}

