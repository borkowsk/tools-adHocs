#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <string.h>
void putmem(char far *p)
{
int i,j;
char bufor1[22],bufor[200];
memset(bufor,1,200);

for(i=1;i<16;i++)
	{
	sprintf(bufor1,"%11Fp ",p);
	for(j=0;j<12;j++)
		bufor[j*2]=bufor1[j];
	for(j=0;j<32;j++)
		bufor[24+j*2]=p[j];
	puttext(1,i,12+32,i,bufor);
	p+=32;
	}
}
main()
{
unsigned long seg=0x0000FFFF,ofs=0x00000000;
char far* p,c;
clrscr();
gotoxy(46,1);cprintf("<,> change sector");
gotoxy(46,2);cprintf("~,/ change offset");
for(int i=0;i<20;i++)
	{
	char bufor1[30];
	sprintf(bufor1,"TEST%d.buf",i);
	FILE* f=fopen(bufor1,"w");
	if(f==NULL) break;
	for(int j=0;j<512;j++)
		fprintf(f,"%2u",i);
	}
gotoxy(46,3);cprintf("%d",i);
do{
p=(char far*)MK_FP(seg,ofs);
putmem(p);
c=getch();
switch(c)
{
case ' ':
case '`':
case '~':
	 ofs+=32;
	 break;
case '/':
	 ofs-=32;
	 break;
case '<':seg+=2;
	break;
case '>':seg-=2;
	break;
case 'p':break;
}
}while(c!=27);
fcloseall();
}