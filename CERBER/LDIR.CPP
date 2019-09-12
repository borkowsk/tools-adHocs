/* findnext example */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <process.h>
#include <dos.h>
#include <dir.h>
struct ffprivate
	{
	unsigned  char disk;/*Numer  przeszukiwanego dysku*/
	char       mask[11];/*Maska przeszukiwania */
	unsigned char m_att;/*Atrybuty przeszukiwania */
	unsigned int  posit;/*Pozycja w katalogu*/
	unsigned int  JAP_k;/*JAP czytanego katalogu*/
	unsigned long LZero;/*Cztery bajty zawsze zerowe*/
	};
char* s_date(char* s,unsigned date)
{
struct dt{
	unsigned dzien:5;
	unsigned miesiac:4;
	unsigned rok:7;
	};
dt dtv=*((dt*)(&date));
sprintf(s,"%4d-%02d-%02d",1980+dtv.rok,dtv.miesiac,dtv.dzien);
return s;
}

char* s_time(char* s,unsigned time)
{
struct tm{
	unsigned sec:5;
	unsigned min:6;
	unsigned godz:5;
	};
tm tmv=*((tm*)(&time));
sprintf(s,"%02d:%02d",tmv.godz,tmv.min);
return s;
}

int main(void)
{
   struct ffblk ffblk;
   struct ffprivate ffp;
   int done;char* what=NULL;
   unsigned int attr;

   if((what=_argv[1])==NULL)
			what="*.*";
   {
   char* end=NULL;
   attr=strtol(_argv[2], &end , 0 );
   }
   printf("Full directory listing %s with attrib 0x%X\n",what,attr);
   done = findfirst(what,&ffblk,attr);
   while (!done)
   {
    int i;
    unsigned int k;
    char d[20/*11*/],t[20/*6*/];

    ffp=*((ffprivate*)(ffblk.ff_reserved));
    printf("%c:%11.11s %2x %3u %6u %8lx ",'@'+ffp.disk,ffp.mask,ffp.m_att,ffp.posit,ffp.JAP_k,ffp.LZero);
    printf("%12s %2x %6lub %s %s\n", ffblk.ff_name,ffblk.ff_attrib,ffblk.ff_fsize,s_time(t,ffblk.ff_ftime),s_date(d,ffblk.ff_fdate));
    done = findnext(&ffblk);
   }
 if (_doserrno != 18)
  {
   fprintf(stderr,"Error:%d ",_doserrno);
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
   return 0;
}
