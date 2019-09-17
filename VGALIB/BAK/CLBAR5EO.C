/*
void _PASCAL_ far clearbar5Eold(long int x1, long int y1,long int x2, long int y2)
{
unsigned int pom, len ;
unsigned int yy1,yy2;

if( x2<x1 || y2<y1 || x2<0L || y2<0L || x1>=640L || y1>=400L ) return;
 x1 = ( x1>  0L ? x1:   0 );
yy1 = ( y1>  0L ? y1:   0 );
yy2 = ( y2<400L ? y2: 399 );
len = ( x2<640L ? x2: 639  )-x1;
UnlockPROA;

while(yy1<=yy2)
	{
	unsigned long int offset=OFFSET5E( x1 , yy1 );

	OutPROA( pom=(offset>>12) ); /* /4096*/

	MEMSET( SCREEN+(offset-(pom<<12)) , 0 , len );
	yy1++;
	}
OutPROA( 0x0 );
RelockPROA;
}
*/