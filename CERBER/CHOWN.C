
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <process.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <ctype.h>

struct f_attrib /* Bajt atrybutow pliku */
	{
	unsigned readonly:1;
	unsigned hidden:1;
	unsigned system:1;
	unsigned label:1;
	unsigned subdir:1;
	unsigned archive:1;
	unsigned free6:1;
	unsigned free7:1;
	};

/* Funkcje pomocnicze */
/*--------------------*/
unsigned error=0;
unsigned Chown(void far* p,unsigned char own)
{
_CH=0;
_CL=own;
asm push ds;
_DX=FP_OFF(p);
_DS=FP_SEG(p);
_AX=0xff02; /* Change owner cerber function */
asm int 21H;
asm jc ERROR;
asm pop ds;/* Restore DATA segment */
return  1;
ERROR:
asm pop ds;/* Restore DATA segment */
_doserrno=error=_AX;
return 0xffff;
}

char* s_date(char* s,unsigned date)
{
struct dt{
	unsigned dzien:5;
	unsigned miesiac:4;
	unsigned rok:7;
	};
struct dt dtv=*((struct dt*)(&date));
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
struct tm tmv=*((struct tm*)(&time));
sprintf(s,"%02d:%02d",tmv.godz,tmv.min);
return s;
}

char* s_attr(char* s,unsigned char attr)
{
struct f_attrib /* Bajt atrybutow pliku */
	{
	unsigned readonly:1;
	unsigned hidden:1;
	unsigned system:1;
	unsigned label:1;
	unsigned subdir:1;
	unsigned archive:1;
	unsigned free6:1;
	unsigned free7:1;
	};
struct f_attrib  fatt=*((struct f_attrib*)(&attr));
sprintf(s,"%c%c%c%c%c%c%c%c",(fatt.readonly?'r':'-')
			    ,(fatt.hidden?'h':'-')
			    ,(fatt.system?'s':'-')
			    ,(fatt.label?'l':'-')
			    ,(fatt.subdir?'d':'-')
			    ,(fatt.archive?'a':'-')
			    ,(fatt.free6?'X':'-')
			    ,(fatt.free7?'Y':'-'));
return s;
}

char users_tab[256][8];/* Tablica identyfikatorow uzytkownikow */
void init_users_tab()/*Funkcja wczytujaca plik passwd */
{
char *s;char drive[MAXDRIVE];char dir[MAXDIR];char file[MAXFILE];char ext[MAXEXT];
char b[MAXPATH];
FILE* passfile;
s=getenv("COMSPEC"); /* get the comspec environment parameter */
fnsplit(s,drive,dir,file,ext);
sprintf(b,"%s%sPASSWD",drive,dir);/* passwd file must by in thy same directory */
if((passfile=fopen(b,"r"))==NULL)
		fprintf(stderr,"LS:Cant open file %s !\n",b);
		else
		{
		char line[128];
		char user[9],pass[9];
		unsigned id;
		while(!feof(passfile))
			{
			fgets(line,128,passfile);
			sscanf(line,"%8s %8s %u ",user,pass,&id);
			strncpy(users_tab[id],user,8);
			}
		fclose(passfile);
		}
}

char* s_id(char* s,unsigned char id)
{
if(id==0)
	sprintf(s," nobody ");
	else
	if(id==1)
	   sprintf(s,"operator");
	   else
	   if(users_tab[id][0]!='\0')
		sprintf(s,"%-8.8s",users_tab[id]);
		else
		sprintf(s,"  %03u  ",id);
return s;
}

unsigned char str2user(const char* s)
{
unsigned char c;
char *end;
if(isdigit(s[0]))
	c=strtol(s, &end , 0 );
	else
	for(c=255;c>0;c--)
	 if(strncmp(users_tab[c],s,8)==0) break;
return c;
}

unsigned str2attr(const char* s)
{
char* end=NULL;
unsigned attr=0;
struct f_attrib  fatt=*((struct f_attrib*)(&attr));

if(isdigit(s[0]))
	{
	attr=strtol(s, &end , 0 );
	return attr;
	}
	else
	while(*s!=0)
		{
		switch(*s)
		 {
		 case 'r':fatt.readonly=1;break;
		 case 'h':fatt.hidden=1;break;
		 case 's':fatt.system=1;break;
		 case 'l':fatt.label=1;break;
		 case 'd':fatt.subdir=1;break;
		 case 'a':fatt.archive=1;break;
		 case 'X':fatt.free6=1;break;
		 case 'Y':fatt.free7=1;break;
		 default:
		   fprintf(stderr,"Invalid attribute swith %c\n",*s);
		 }
		s++;
		}
attr=*((unsigned char*)(&fatt));
return attr;
}

int GetUser_Id(void)
{
_AX=0xFF00;
geninterrupt(0x21);
if(_AX==0x0F0F) return _CX;
	else    return -1;
}

int main(void)
{
   struct ffblk ffblk;
   char s[MAXPATH];
   char drive[MAXDRIVE];
   char dir[MAXDIR];
   char file[MAXFILE];
   char ext[MAXEXT];

   int done=0,wyn=0,len=0;
   char* what=NULL;
   unsigned int attr=0x10;
   unsigned int owner_id=0; /* Nowy uzytkownik dla plikow ! */
   unsigned int user_id=GetUser_Id();/* Aktualnie pracujacy uzytkownik */
   char a[20/*9*/],u[20],new_owner[20];
   if(_argv[1]==NULL)
	{
	printf("USAGE:>  chown user [file_mask] [attribute_mask]\n");
	exit(1);
	}
   if(user_id==0xFFFF)
	{
	fprintf(stderr,"CERBER is not active !\n");
	user_id=0;
	exit(1);
	}

   init_users_tab();
   owner_id=str2user(_argv[1]);
   if((what=_argv[2])==NULL)
			what="*.*";
   if(_argc>=4)
       attr=str2attr(_argv[3]);

   printf("CHANGING OWNER TO %s FOR FILES %s %s \n",s_id(new_owner,owner_id),what,attr!=0?s_attr(a,attr):" ");

   fnsplit(what,drive,dir,file,ext); /* split the string to separate elems */
   len=strlen(what)+8;

   done = findfirst(what,&ffblk,attr);
   while (!done)
   {
    int i;
    sprintf(s,"%s%s%s",drive,dir,ffblk.ff_name);   /* merge everything into one string */
    printf("%-*s ",len,s);
    wyn=Chown((char far* )s,owner_id);
    if( wyn==1 )
	printf("\t OK !\n");
	else
	printf("\t ACCESSION FAIL ! (code %d) \n",_doserrno);
    done = findnext(&ffblk);
   }
return 0;
ERROR:
   fprintf(stderr,"Error:%d ",_doserrno);
   perror("");
return 1;
}
