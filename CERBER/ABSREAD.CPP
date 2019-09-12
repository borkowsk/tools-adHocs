/* absread example */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <process.h>
#include <dos.h>
#include <ctype.h>

int absread(int drive, int nsects, long lsect, void *buffer)
{
struct par { unsigned long lsect;
		       int nsect;
	       void far * buffer;
	   }  ;
par param;
int bladv;

param.lsect=lsect;
param.nsect=nsects;
param.buffer=buffer;
_BX=FP_OFF(&param);
_DS=FP_SEG(&param);
_CX=-1;
_AL=drive;
asm{	int 25H
	jc blad
	popf}
return 0;
blad:
bladv=_AX;
asm popf     ;
errno=_doserrno=bladv;
return -1;
}

int main(void)
{
  int i,j, strt, ch_out, sector;
  char buf[512],*format=NULL;
  if(_argc<3)
	{
	printf("USAGE:>   ABSREAD  disk_letter  sector_number [c-format] \n");
	return 1;
	}
/*  i=atoi(_argv[1]);*/	/* disk_number */
  i=toupper(_argv[1][0])-'A';/* Disk letter to number */
  sector=atoi(_argv[2]);/* sector_number */
  if((format=_argv[3])==NULL)
			format="%c";
  printf("Reading sector %d from disk %d=%c ",sector,i,_argv[1][0]);
  if (absread(i, 1, sector, &buf) != 0)
  {
   fprintf(stderr,"DOS error:%d ",_doserrno);
   perror("");
   struct DOSERROR info;
   dosexterr(&info);
   printf("Extended DOS error information:\n");
   printf("   Extended error:     %d\n",info.de_exterror);
   printf("            Class:     %x\n",info.de_class);
   printf("           Action:     %x\n",info.de_action);
   printf("      Error Locus:     %x\n",info.de_locus);
   exit(1);
  }
  printf("Read OK\n");
  strt = 0;
for (i=0; i<16; i++)
 {
 for(j=0;j<32;j++)
	  {
	  printf(format,(unsigned char)buf[strt+j]);
	  }
 strt+=32;
 printf("\n");
 }

  return(0);
}
