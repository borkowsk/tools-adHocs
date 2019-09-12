/*
Biblioteka BIOSGRAPH:
Funkcja liczaca sqrt z unsigned long int i wyprowadzajaca unsigned int
*/
#include <math.h>

unsigned int isqrt(unsigned long int par)
{
unsigned int Max=0xFFFF,Min=0x0,Bis;
unsigned long int  BisBis;
do
{
Bis= Max+Min>>1;
BisBis=(unsigned long)Bis*Bis;
if( BisBis<=par ) Min=Bis;
	else	  Max=Bis;


}
while( Min!=Max-1 );
return Min;

/*if( par-(unsigned int)Min*Min < (unsigned int)Max*Max-par )
	return Min;
	else
	return Max; */
}

unsigned int _c_isqrt(unsigned long int par)
{
return sqrt(par);/* Coprocesor */
}


