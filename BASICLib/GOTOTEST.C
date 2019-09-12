#include <stdio.h>
main()
{
int i=1,oj=2;
{
	int j;
	j=oj;
	i=j;
	goto kl;
jl:
	printf("1)j winno byc =%d\n",oj);
	printf("2)j jest  =%d\n",j);
}
return 0;
kl:
{
	int k;
	k=i+2;
	printf("0)k jest =%d\n",k);
	goto jl;
}
}