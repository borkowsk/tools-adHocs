/* KONWERSJA BAJTU NA TABLICE OSMIU BAJTOW */
/*                      i                  */
/*          TABLICY BAJTOW NA BAJT         */

char pack(char* bool)
{
/* bool ARRAY 0 or 1 */
char pom=0x0;
pom|=bool[0];
pom|=(bool[1]<<1);
pom|=(bool[2]<<2);
pom|=(bool[3]<<3);
pom|=(bool[4]<<4);
pom|=(bool[5]<<5);
pom|=(bool[6]<<6);
pom|=(bool[7]<<7);
return pom;
}

char* unpack(char* bool,char pom)
{
bool[0]=(pom &  0x01);
bool[1]=(pom &  0x02)>>1;
bool[2]=(pom &  0x04)>>2;
bool[3]=(pom &  0x08)>>3;
bool[4]=(pom &  0x10)>>4;
bool[5]=(pom &  0x20)>>5;
bool[6]=(pom &  0x40)>>6;
bool[7]=(pom &  0x80)>>7;
return bool;
}

char* bool2chars(char* printable,char* bool)
{
printable[7]=(bool[0]?'1':'0');
printable[6]=(bool[1]?'1':'0');
printable[5]=(bool[2]?'1':'0');
printable[4]=(bool[3]?'1':'0');
printable[3]=(bool[4]?'1':'0');
printable[2]=(bool[5]?'1':'0');
printable[1]=(bool[6]?'1':'0');
printable[0]=(bool[7]?'1':'0');
printable[8]='\0';
return printable;
}

/*
main()
{
char bity[]={1,1,1,1,1,1,1,1};
char nbity[8];
char vbity[9];
char wbity[9];
unsigned char byte;
unsigned i,y;

byte= pack(bity);
unpack(nbity,byte);
printf(" %s --> %u --> %s \n",bin2chars(vbity,bity),byte,bin2chars(wbity,nbity));
for( i=0;i<100;i++)
	{
	unpack(nbity,i);
	y=pack(nbity);
//      printf("%u --> %s --> %u \n",i,bool2chars(wbity,nbity),y);
	}
if(bity[1])
	 ;
return  1;
}
*/
