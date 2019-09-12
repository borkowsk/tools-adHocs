#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include <dir.h>
#include <conio.h>


char far* LLpointer()
{
asm mov bx,0;
asm mov ah,52H;
asm int 21H; //Change es & bx
return  (char far*)MK_FP(_ES,_BX);
}

unsigned realDOSversion()
{
asm mov bx,0;
asm mov ah,33H;
asm mov al,06H;
asm int 21H;
asm mov al,bh;
asm mov ah,bl;
return _AX;
}

#define hi(_I_)  (((_I_)&0xFF00)>>8)
#define lo(_I_)  ((_I_)&0x00FF)

struct BUF{
unsigned first;
unsigned second;
unsigned thirst;
unsigned long sector;
unsigned inf[5];
char val[512];
};

struct BINF{
struct BUF far* P;
unsigned pn[20];
};

struct LLst{
unsigned licz;
unsigned delay;
struct BUF far* curbuf;
unsigned conbuf;
unsigned firstMCB;
};

struct LL{
void far* 	FirstDPB;
void far* 	SFT;
void far* 	CLOCKS;
void far* 	CON;
unsigned  	blocksize;
struct BINF far* buffinfo;/* Informacja o buforach */
//void far*       buffinfo;
void far* 	CDS;   /* tablica katalogow aktualnych */
void far* 	FCBST; /* tablica blokow opisu plikow */
unsigned  	FCBreserved;
unsigned char 	NumberBlockDevices;
unsigned char 	LASTDRIVE;
unsigned char 	NULheader[18];
unsigned char 	NumberJoinedDisk;
void near*    	SpecialProgramNames;/* Lista nazw specjalnych programow - offset w kodzie IBMDOS */
void far* 	ServisesProc;/* Procedura z uslugami pomocniczymi */
void far* 	ChainOfIFSdevices;/* Installable File Systems */
unsigned  	bufnum;/* Ilosc buforow */
unsigned  	SecondaryBufNum;/* Ilossc buforow pamieci wtornej */
unsigned char  	StartingDisk;/* Dysk z ktorego zaladowano system */
unsigned char  	reserved;
unsigned  	ExtendedMemory;
};
struct LL far* LL=(struct LL far*)LLpointer();
struct LLst far* LLst=(struct LLst far* )(LLpointer()-0x0C);
unsigned char far* InDos=0;

int checkdirectory(int,char *,char *);

main()
{
int i,h;
unsigned filenumb=0;
unsigned rversion=realDOSversion();
_AH=0x34;geninterrupt(0x21);
InDos=(unsigned char far*)MK_FP(_ES,_BX);
textcolor(WHITE);
clrscr();
printf("\n\nSystem checker for instaling CERBER utility\n");
printf("--------------------------------------------\n");
printf("OS version \t\t%u.%u (mayby from SETVER)\n",_osmajor,_osminor);
printf("Real DOS version \t%u.%u",hi(rversion),lo(rversion));
if(_osmajor<4 &&(hi(rversion)<5 || hi(rversion)>10))
	{printf(" Sorry - too old DOS system !");exit(1);}
if( hi(rversion)==4 || hi(rversion)==5)
	{printf(" System version is compatible.");}
if(hi(rversion)>5)
	{printf(" Too new version of system - not tested !!!");}
putchar('\n');
printf("\nInformation from DOS LL structure - see it !  \n");
printf("---------------------------------------------\n");
printf("LL at \t\t\t\t%Fp\n",LL);
printf("Size of disk blocks \t\t%u\n",LL->blocksize);
printf("Last drive letter \t\t%c\n",'@'+LL->LASTDRIVE);
printf("Number of block devices \t%u\n",LL->NumberBlockDevices);
printf("Number of joined disk \t\t%u\n",LL->NumberJoinedDisk);
printf("Number of BUFFERS \t\t%u\n",LL->bufnum);
printf("Number of secondary BUFFERS \t%u",LL->SecondaryBufNum);
if(LL->SecondaryBufNum!=0)
	printf(" Use rather SmartDrive !");
putchar('\n');
printf("Current BUFFERS at \t\t%Fp\n",(LL->buffinfo)->P);
printf("System disk letter \t\t%c\n",'@'+LL->StartingDisk);
printf("Extended memory \t\t%uKB \n",LL->ExtendedMemory);
gotoxy(1,25);clreol();getch();
cprintf("Checking hard disks and direct buffer access .");
//printbuf((LL->buffinfo)->P,LL->bufnum);
filenumb=0;
for(i='C';i<'A'+LL->LASTDRIVE;i++)
	{
	filenumb+=checkdirectory(i,"\\","*.*");
	gotoxy(1,25);clreol();
	cprintf("%u file(s)",filenumb);delay(500);
	}
gotoxy(1,25);clreol();
cprintf("Tested %u file(s) - if no errors - CERBER is compatible .",filenumb);getch();
return 1;
}

void putmem(char far *p, int s)
{
int i,j;
char bufor1[22],bufor[200];
memset(bufor,1,200);

for(i=s;i<s+16;i++)
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

/* Struktury systemowe */
/*---------------------*/
/*
struct ffblk {  /* DOS file control block structure. */
    char      ff_reserved[21];
    char      ff_attrib;
    unsigned  ff_ftime;
    unsigned  ff_fdate;
    long      ff_fsize;
    char      ff_name[13];
    };
*/
struct ffprivate /* Rekord transmisyjny miedzy funkcja FindFirst i FindNext */
	{
	unsigned  char disk;/*Numer  przeszukiwanego dysku*/
	char       mask[11];/*Maska przeszukiwania */
	unsigned char m_att;/*Atrybuty przeszukiwania */
	unsigned int  posit;/*Pozycja w katalogu*/
	unsigned int  JAP_k;/*JAP czytanego katalogu*/
	unsigned long LZero;/*Cztery bajty zawsze zerowe*/
	};
#define SUBDIR 16
#define LABEL  8
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
	unsigned int  fJAP;
	unsigned long fsize;
	};

char* name_from_curbuf(char* target,struct ffblk volatile * ff)
{

unsigned flag=0,oldi=0,i=0,j=0,posit=((ffprivate*)ff)->posit;
char far* entry;
if(ff->ff_name[0]=='.'|| (ff->ff_attrib & LABEL))
	{
	strcpy(target,ff->ff_name);
	return target;
	}
posit%=16;	/* In current segment */
entry=(LLst->curbuf)->val+posit*32;
while(entry[j]!=' '&& j<8 )/* Copy name */
	{target[i]=entry[j];
	 i++;j++;}
oldi=i;		/* Save position */
while(j<8 )/* Copy illegal tail with spaces */
	{if( (target[i]=entry[j])!=' ')
		flag=1;
	i++;j++;}
if(!flag)
	i=oldi;/* Restore position */
	else
	{
	gotoxy(1,2);clreol();
	highvideo();
	cprintf("Warning:\07Spaces in DOS names are illegal !%11.11Fs",entry);
	delay(500);
	normvideo();
	}
j=8;	         /* First char of extension */
if(entry[j]!=' ')
	{target[i]='.';i++;}
while(entry[j]!=' ' && j<11)/* Copy extension */
	{target[i]=entry[j];
	 i++;j++;}
target[i]='\0';
return target;
}

int checkdirectory(int i,char *path,char *mask)
{
struct ffblk ffblk;
int done=0;
int number=0;
char strbuf[128];
sprintf(strbuf,"%c:%s%s",i,path,mask);
done = findfirst(strbuf,&ffblk,255);
while(!done){
	number++;
	sprintf(strbuf,"%c:%s%s",i,path,ffblk.ff_name);
	_chmod(strbuf,0,0);
	(*InDos)++;
	name_from_curbuf(strbuf,&ffblk);
	(*InDos)--;
	gotoxy(1,1);clreol();
	if(strcmp(ffblk.ff_name,strbuf)==0)
		{
		cprintf("%c:%s%s=%s",i,path,ffblk.ff_name,strbuf);
		if((ffblk.ff_attrib & SUBDIR) && ffblk.ff_name[0]!='.')
			{
			char newpath[128];
			sprintf(newpath,"%s%s\\",path,ffblk.ff_name);
			number+=checkdirectory(i,newpath,mask);
			}
		}else
		{
		int odp=0;
		gotoxy(1,1);clreol();
		cprintf("%c:%s%s<>\"%s\"[%u]",i,path,ffblk.ff_name,strbuf,((ffprivate*)&ffblk)->posit);
		gotoxy(1,3);clreol();
		cprintf("Erorr!\07 Name from bufer is nonequal ! Break ? (y/n)");
		putmem((char far*)LLst->curbuf,4);
		odp=getch();
		if(odp=='y')
			{exit(1);}
		gotoxy(1,3);clreol();
		}
	done = findnext(&ffblk);
	}
return number;
}