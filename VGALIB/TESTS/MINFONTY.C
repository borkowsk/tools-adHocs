#include<conio.h>
#include<stdio.h>
#include "biosgraf.h"

void fonty(unsigned char font[256][12])
{
FILE *fp;
if((fp=fopen("vgatest\\fonty.dat","rb"))==(FILE*)NULL)
cprintf("\n ZLE");
else{
 fread(font,12,256,fp);
 fclose(fp);
  }
}

unsigned char font[256][12];

void litera(int kod,int x,int y,int kolor)
{
char far *farptr;

int i,j;

farptr = (char far *) 0xa0000000L;
   farptr+=y*320+x;

for(i=0;i<12;i++)
  {
   for(j=7;j>=0;j--)
     {
      if(((font[kod][i])&(1<<j))!=0)*farptr=kolor;
      farptr++;
      }
   farptr+=312;
   }
}

void MakeCfont(char* name)
{
FILE *fp;
int i,j;
if((fp=fopen(name,"w"))==NULL)
cprintf("\n ZLE");
else{
fprintf(fp,"{\n");
for(i=0;i<256;i++)
	for(j=0;j<12;j++)
		{
		fprintf(fp,"0x%x",font[i][j]);
		if(j!=11) fprintf(fp,",");
		   else   fprintf(fp,",\n");
		}
fprintf(fp,"};\n");
fclose(fp);
}
}

main()
{
int i,j,l=0;
fonty(font);
BiosScreen(0x13);
for(i=0;i<8;i++)
	for(j=0;j<36;j++)
	  litera(l++,8*j,20*i,15);
litera('S',10,10,15);
litera('A',20,10,15);
litera('V',30,10,15);
litera('E',40,10,15);
litera('!',50,10,15);
MakeCfont("fonty.inc");
litera('O',10,110,15);
litera('K',20,110,15);
litera('!',30,110,15);
litera('!',40,110,15);
getch();
BiosScreen(0x3);
}

/*
void litera(int kod,int x,int y,int kolor){
char far *farptr;
extern unsigned char font[256][12];
int i,j;

farptr = (char far *) 0xa0000000L;
   farptr+=y*320+x;

for(i=0;i<12;i++)
  {
   for(j=7;j>=0;j--)
     {
      if(((font[kod][i])&(1<<j))!=0)*farptr=kolor;
      farptr++;
      }
   farptr+=312;
   }
}

*/
