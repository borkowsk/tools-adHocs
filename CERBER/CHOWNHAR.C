
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <process.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <ctype.h>

/* Struktury systemowe */
/*---------------------*/
struct ffprivate /* Rekord transmisyjny miedzy funkcja FindFirst i FindNext */
	{
	unsigned  char disk;/*Numer  przeszukiwanego dysku*/
	char       mask[11];/*Maska przeszukiwania */
	unsigned char m_att;/*Atrybuty przeszukiwania */
	unsigned int  posit;/*Pozycja w katalogu*/
	unsigned int  JAP_k;/*JAP czytanego katalogu*/
	unsigned long LZero;/*Cztery bajty zawsze zerowe*/
	};

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

struct dir_entry /* Rekord katalogu */
	{
	char name[8];
	char ext[3];
	struct f_attrib attr;
	unsigned char user_id;
	char free[9];
	unsigned int  ftime;
	unsigned int  fdate;
	unsigned int   fJAP;
	unsigned long fsize;
	};

struct DPB /* Blok parametrow dysku */
	{
	unsigned char DiscNum;/* Numer dysku */
	unsigned char InObsNum;/* Numer jednostki w programie obslugi */
	unsigned int  SecSize;/* Wielkosc sektora w bajtach */
	unsigned char SecJAP_1;/* Liczba sektorow w JAP-1 */
	unsigned char logSecJAP;/* log2 z liczby sektorow w JAP */
	unsigned int  SecReserved;/* Liczba sektorow zarezarwowanych przed FAT-em */
	unsigned char FATnum; /* Liczba kopi FAT */
	unsigned int  FileMax; /* Maksymalna liczba plikow w katalogu glownym */
	unsigned int  FirstData; /*Pierwszy sektor danych */
	unsigned int  MaxJAP; /* Najwiekszy numer JAP - liczba JAP przeznaczonych na dane +1*/
	unsigned int  FATSize; /* Liczba sektorow zajertych przez tablice FAT */
	unsigned int  RootFirst; /* Numer pierwszego sektora katalogu */
	void far *    Procedure; /* Wskaznik do programu obslugi dysku */
	unsigned char MediaBayt; /* Bajt identyfikacji nosnika */
	unsigned char AccesBayt; /* Bajt dostepu do dysku - 0xFF - nie zadano */
	struct DPB far*      Follow;    /* Wskaznik do nastepnego DPB - 0xffff -ostatni */
	unsigned int  FistUnUse; /* Pierwsza wolna JAP na dysku */
	unsigned int  UnUsedJAP; /* Liczba wolnych JAP na dysku - 0xffff - nie znana */
	};



/* Funkcja pobierania bloku parametrow dysku */
/*-------------------------------------------*/
struct DPB far* GetDPB(unsigned char D)
{
struct DPB far* retval;
unsigned char bl;

_DL=D;
_AH=0x32;
asm push ds;/* Save DATA segment */
geninterrupt(0x21);
bl=_AL;
retval=(struct DPB far*)MK_FP(_DS,_BX);
asm pop ds;/* Restore DATA segment */
if(bl!=0) return NULL;
   else   return retval;
}

/* Funkcja czytania sektora z dysku */
/*----------------------------------*/
int AbsRead(int drive, int nsects,unsigned long lsect, void *buffer)
{
struct par { unsigned long lsect;
		       int nsect;
	       void far * buffer;
	   }  ;
struct par param;
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
//error=bladv;
return -1;
}

int AbsWrite(int drive, int nsects,unsigned long lsect, void *buffer)
{
struct par { unsigned long lsect;
		       int nsect;
	       void far * buffer;
	   }  ;
struct par param;
int bladv;
param.lsect=lsect;
param.nsect=nsects;
param.buffer=buffer;
_BX=FP_OFF(&param);
_DS=FP_SEG(&param);
_CX=-1;
_AL=drive;
asm{	int 26H
	jc blad
	popf}
return 0;

blad:
bladv=_AX;
asm popf     ;
//error=bladv;
return -1;
}

/* Cache do czytania FAT-u */
/*-------------------------*/
unsigned FATbuf[128];	/* Bufor do wczytywania sektorow FATu */
char diskFAT=0xff;  	/* Z ktorego dysku jest aktualnie wczytany sektor */
unsigned long old_sectorFAT=0xffffffff;/* Numer aktualnie wczytanego sektora */
extern char read_req; /* Flaga nieaktualnosci informacji, ta co dla katalogow*/

unsigned NextJAP(struct DPB far* ThisDPB,unsigned  JAP)
{
unsigned sector;
unsigned poz;
if(ThisDPB->MediaBayt==0xF8/*Dysk twardy FAT16 */)
{
sector=(JAP/256)+ThisDPB->SecReserved;
poz=(JAP%256);
if(ThisDPB->DiscNum !=diskFAT || sector!=old_sectorFAT || read_req)
	{
	if( AbsRead(ThisDPB->DiscNum, 1, sector, &FATbuf)!= 0)
	       return 0x0FF1;/* Blad odczytu */
	       else
	       {
	       diskFAT=ThisDPB->DiscNum;
	       old_sectorFAT=sector;
	       }
	}
return FATbuf[poz];
}
else /* Najpewniej dyskietka - FAT12*/
{
unsigned pom;
poz=(JAP*3)/2; /* <=> Trunc(JAP*1.5) */
sector=(poz/512)+ThisDPB->SecReserved;
if(ThisDPB->DiscNum !=diskFAT || sector!=old_sectorFAT || read_req)
	{
	if( AbsRead(ThisDPB->DiscNum, 1, sector, &FATbuf)!= 0)
	       return 0x0FF1;/* Blad odczytu */
	       else
	       {
	       diskFAT=ThisDPB->DiscNum;
	       old_sectorFAT=sector;
	       }
	}
poz%=512;/* In read sector */
pom=JAP;
JAP=*((unsigned*)( ((char*)FATbuf+poz) ));
if(pom&1!=0)
	{
	JAP&=0xFFF0;
	JAP>>=4;
	return JAP;
	}
	else
	{
	JAP&=0x0FFF;
	return JAP;
	}
}
}

int ChainDirFAT(struct DPB far* ThisDPB,unsigned  *SJAP,unsigned  *SEnt)
{
#define JAP (*SJAP)
#define Ent (*SEnt)
unsigned i,n=(Ent/16)/(ThisDPB->SecJAP_1+1); /* Okresla ile FATentries trzeba przeczytac zeby dotrzec do wlasciwej */
for(i=1;i<=n;i++)/* Moze nie wykonac sie ani razu jesli sektor jest w podanej JAP */
	{
	JAP=NextJAP(ThisDPB,JAP);
	if((JAP&0x0FFF)>=0x0FF0 || JAP==0) /* Blad odczytu lub parametrow */
		return 0;
	Ent-=(16*(ThisDPB->SecJAP_1+1));
	}
return 1;
#undef JAP
#undef Ent
}

/* Cache do pobierania sektorow katalogow */
/*----------------------------------------*/
struct dir_entry bufor[16]; /* Bufor na wczytywanie segmentow katalogow */
char read_req=1; /* Flaga nieaktualnosci informacji */
char write_req=0;/* Flaga koniecznosci zapisu */
char disk=0xff;  /* Z ktorego dysku jest aktualnie wczytany sektor */
unsigned long old_sector=0xffffffff;/* Numer aktualnie wczytanego sektora */

int  loaddirsec(unsigned char Disc,unsigned int JAP,unsigned* SEnt)/* Ladowanie - zwraca 1 jesli blad */
{
#define Ent  (*SEnt)
struct DPB far* ThisDPB=GetDPB(Disc);
unsigned long sector=0xffffffff;

if(ThisDPB==NULL || (ThisDPB->SecSize>512) )
	return  1;

if(JAP==0)
 sector=ThisDPB->RootFirst;
 else
 {
 if(ChainDirFAT(ThisDPB,&JAP,&Ent)==0)
		return  1;
 sector=((unsigned long)JAP-2)*(ThisDPB->SecJAP_1+1)+ThisDPB->FirstData;
 }
 sector+=(Ent/16); /* Powieksza jesli nie w piwrwszym sektorze JAP */
 Ent=(Ent%16);	   /* Pozycja we wczytanym sektorze */

 if(sector!=old_sector || read_req)/* Czytanie gdy inny sektor lub mozliwosc dezaktualizacji */
	{
	writedirsec();
	if( AbsRead(Disc-1, 1, sector, &bufor)!= 0)
	       return 1;
	       else{
		disk=Disc;/* Zapamientanie parametrow wczytanego sektora */
		old_sector=sector;
		read_req=0;
		write_req=0;
		}
	}
return 0;
#undef Ent
}

int writedirsec()/* Aktualizuje obraz dyskowy sektora z Cach'u - zwraca 1 jesli nie wykonal*/
{
if(write_req && old_sector!=0xffffffffL && disk!=0xff)/* Jesli bylo cos zmieniane to trzeba zapisac */
    if( AbsWrite(disk-1,1,old_sector,&bufor) !=0 )
	      return 1;
write_req=0;
return 0;
}

/* Funkcje pomocnicze */
/*--------------------*/
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
   struct ffprivate ffp;
   struct dir_entry* dent=NULL;
   int done;char* what=NULL;
   unsigned int attr=0x0;
   unsigned int owner_id=0; /* Nowy uzytkownik dla plikow ! */
   unsigned int user_id=GetUser_Id();/* Aktualnie pracujacy uzytkownik */
   char a[20/*9*/],u[20],new_owner[20];
   if(_argv[1]==NULL)
	{
	printf("USAGE:>  chown user [file_mask] [attribute_mask]\n");
	exit(1);
	}
   if(user_id==0xFFFF) {fprintf(stderr,"CERBER is not active !\n");user_id=0;}
   init_users_tab();
   owner_id=str2user(_argv[1]);
   if((what=_argv[2])==NULL)
			what="*.*";
   if(_argc>=4)
       attr=str2attr(_argv[3]);

   printf("CHANGING OWNER TO %s FOR FILES %s %s (y/n)\n",s_id(new_owner,owner_id),what,attr!=0?s_attr(a,attr):" ");
   if(getch()!='y')
	   exit(1);

   done = findfirst(what,&ffblk,attr);
   while (!done)
   {
    int i;
    unsigned int posit;
    ffp=*((struct ffprivate*)(ffblk.ff_reserved));
    posit=ffp.posit;
    if((done=loaddirsec(ffp.disk,ffp.JAP_k,&posit))!=0)/* Ladowanie - zwraca 1 jesli blad */
				goto ERROR;/* Tedy wyjscie z petli !*/
    dent=((struct dir_entry*)bufor+posit);
    printf("%3u>%8.8s %3.3s %8s->%8s",
      ffp.posit,dent->name,dent->ext,s_id(u,dent->user_id),new_owner);
    if(dent->user_id==0 || dent->user_id==user_id || user_id==1)
	{
	dent->user_id=owner_id;
	write_req=1;/* Zmieniono wiec wymaga zapisu */
	printf(" OK !\n");
	}
	else
	printf(" ACCESSION FAIL !\n");
    done = findnext(&ffblk);
   }
 writedirsec();
ERROR:
 if (_doserrno != 18)
  {
   struct DOSERROR info;
   fprintf(stderr,"Error:%d ",_doserrno);
   perror("");
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
