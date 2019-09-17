void far FillCircle5Eold(long int xxs,long int yys,long int bb)
{
unsigned long int offset=0L,pom;
int xs=xxs;
int ys=yys;
int b=bb;
int y,x,xa,ya,xe,a;
long int y2,a2,b2;
a=b;
b*=0.80 ;
b2=(long int)b*b;
a2=(long int)a*a;
UnlockPROA;
for(y=0;y<b;y++)
	{
	y2=(long int)y*y;
	x=sqrt((a2*b2-y2*a2)/b2);
	xe=xs+x;
	if(xe>639 ) xe=639;
	xa=xs-x;
	if(xa<0) xa=0;
	for(;xa<xe;xa++)
		{
		ya=ys+y;
		if(ya>=0 && ya<=399)
			{
			offset=OFFSET5E( xa , ya );
			OutPROA( pom=(offset>>12) );		 /* /4096 */
			*( SCREEN+(offset-(pom<<12)) )=COLOR;	 /* %4096 */
			}
		ya=ys-y;
		if(ya>=0 && ya<=399)
			{
			offset=OFFSET5E( xa , ya );
			OutPROA( pom=(offset>>12) );		 /* /4096 */
			*( SCREEN+(offset-(pom<<12)) )=COLOR;	 /* %4096 */
			}
		}
	}
OutPROA( 0x0 );
RelockPROA;
}