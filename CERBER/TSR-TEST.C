/***NOTE:
   This is an interrupt service routine.  You
   can NOT compile this program with Test
   Stack Overflow turned on and get an
   executable file which will operate
   correctly.
*/
/*#pragma inline*/
#include <dos.h>
/*#include <ctype.h>*/

/* The clock tick interrupt */
#define INTR 0x1C
#define INTDOS 0x21
/* Screen attribute (blue on grey) */
#define ATTR 0x0F00

/* reduce heaplength and stacklength
to make a smaller program in memory */
extern unsigned _heaplen = 0/*512*/;/* Nie potrzebna ?*/
extern unsigned _stklen  = 512;
int Cerber21H();/* Funkcja oblugujaca */

unsigned int (far *screen)[80];
void interrupt ( *oldhandler)();
void interrupt ( *oldDOShandler)();
unsigned char far* InDos=0;
unsigned int Stack=0;
unsigned int MySP=0;
unsigned int CallerStack=0;
unsigned int CallerSP=0;
/* STRUKTURA DO PRZECHOWYWANIA REJESTROW WYWOLANIA PO ZMIANIE STOSOW */
struct REGPACK r;

const char blip[]="!/-\­/-\!";
const int lblip=sizeof(blip)-1;
int pblip=0;

#define U unsigned
void interrupt DOShandler(U bp,U di,U si,U ds,U es,U dx,U cx,U bx,U ax,U ip,U cs,U flags)
#undef U
{
CallerStack=_SS;
CallerSP=_SP;
pblip=0;
if(ax==0x4301 || ax>0xff00)
	{
	screen[0][79] ='X' + ATTR;
	/* Zapamientanie rejestrow */
	r.r_ax=ax;r.r_bx=bx;r.r_cx=cx;r.r_dx=dx;
	r.r_si=si;r.r_di=di;r.r_ds=ds;r.r_es=es;
	r.r_flags=flags;r.r_bp=bp;
	/* Przelanczanie stosu do stanu z MAIN */
	_SS=Stack;
	_SP=MySP;
	/* Funkcja obslugujaca z casem w srodku */
	Cerber21H();
	_SS=CallerStack;
	_SP=CallerSP;
	/* Odtworzenie rejestrow */
	ax=r.r_ax;bx=r.r_bx;cx=r.r_cx;dx=r.r_dx;
	si=r.r_si;di=r.r_di;ds=r.r_ds;es=r.r_es;
	flags=r.r_flags;bp=r.r_bp;
	return;
	}
	else
	{
	screen[0][79] ='?' + ATTR;
	}
/* Przekazanie sterowania pod adres orginalny */
asm{
pop bp;
pop di;
pop si;
pop ds;
pop es;
pop dx;
pop cx;
pop bx;
pop ax;
jmp cs:[oldDOShandler];/* Tylko model TINY !!! */
}
}

void interrupt handler()
{
pblip++;
pblip%=lblip;
/* put the number on the screen */
 if(*InDos!=0)
   screen[0][79] = blip[pblip] + ATTR;
   else
   {
   if(pblip==0)
	screen[0][79]='+' + ATTR;
   }
/* call the old interrupt handler */
   oldhandler();
}

int main(void)
{
void far* moj_adres;

Stack=_SS;
MySP=_SP;
/* For a color screen the video memory
   is at B800:0000.  For a monochrome
   system use B000:000 */
(void far* )screen = MK_FP(0xB000,0);
	_AH=0x34;geninterrupt(0x21);
InDos=MK_FP(_ES,_BX);

/* get the addreses of interrups */
oldhandler =getvect(INTR);
oldDOShandler =getvect(INTDOS);

/* install the new interrupt handler */
setvect(INTR, handler);
setvect(INTDOS, DOShandler);

/* _psp is the starting address of the
   program in memory.  The top of the stack
   is the end of the program.  Using _SS and
   _SP together we can get the end of the
   stack.  You may want to allow a bit of
   saftey space to insure that enough room
   is being allocated ie:
   (_SS + ((_SP + safety space)/16) - _psp)
*/
keep(0, (_SS + (_SP/16) - _psp));/* Juz nie wraca z tego wywolania */
return 0;
}

/* BLOK DEFINICJI FUNKCJI IMPLEMENTUJACYCH DZIALANIA TSR-a */
#define CARRY 0x01
unsigned char User_Id=0x0;/* Identyfikator aktualnie pracujacego uzytkownika */

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
	f_attrib attr;
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
	DPB far*      Follow;    /* Wskaznik do nastepnego DPB - 0xffff -ostatni */
	unsigned int  FistUnUse; /* Pierwsza wolna JAP na dysku */
	unsigned int  UnUsedJAP; /* Liczba wolnych JAP na dysku - 0xffff - nie znana */
	};

/* Funkcja pobierania bloku parametrow dysku */
/*-------------------------------------------*/
DPB far* GetDPB(unsigned char D)
{
DPB far* retval;
unsigned char bl;

_DL=D;
_AH=0x32;
asm push ds;/* Save DATA segment */
geninterrupt(0x21);
bl=_AL;
retval=(DPB far*)MK_FP(_DS,_BX);
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

int AbsWrite(int drive, int nsects,unsigned long lsect, void *buffer)
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
asm{	int 26H
	jc blad
	popf}
return 0;
blad:
bladv=_AX;
asm popf     ;
errno=_doserrno=bladv;
return -1;
}

/* Cache do pobierania sektorow katalogow */
/*----------------------------------------*/
char read_req=1; /* Flaga nieaktualnosci informacji */
char write_req=0;/* Flaga koniecznosci zapisu */
char disk=0xff;  /* Z ktorego dysku jest aktualnie wczytany sektor */
unsigned long old_sector=0xffffffffL;/* Numer aktualnie wczytanego sektora */
char bufor[512]; /* Bufor na wczytywanie segmentow katalogow */

int writedirsec()/* Aktualizuje obraz dyskowy sektora z Cach'u - zwraca 1 jesli nie wykonal*/
{
if(write_req && old_sector!=0xffffffffL && disk!=0xff)/* Jesli bylo cos zmieniane to trzeba zapisac */
    if( AbsWrite(disk-1,1,old_sector,&bufor) !=0 )
	      return 1;
write_req=0;
return 0;
}

int  loaddirsec(unsigned char Disc,unsigned int JAP,unsigned int &Ent)/* Ladowanie - zwraca 1 jesli blad */
{
DPB far* ThisDPB=GetDPB(Disc);
unsigned long sector=0xffffffff;

if(ThisDPB==NULL || (ThisDPB->SecSize>512) )
	return  1;
if(JAP==0)
 sector=ThisDPB->RootFirst;
 else
 sector=((unsigned long)JAP-2)*(ThisDPB->SecJAP_1+1)+ThisDPB->FirstData;
 sector+=(Ent/16); /* Powieksza jesli nie w pierwszym sektorze katalogu */
 Ent=(Ent%16);	   /* Pozycja we wczytanym sektorze */

if(sector!=old_sector || read_req)/* Czytanie gdy inny sektor lub mozliwosc dezaktualizacji */
	{
	if(writedirsec()) return 1;/* Aktualizacja jesli trzeba - wypada jesli blad ! */
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
}
/* Glowna funkcja sterujaca praca TSR-a */
int Cerber21H()
{
switch(r.r_ax){
case 0xff00:r.r_ax=0x0f0f;r.r_cx=User_Id;	/*FF00H - Get User_ID 	*/
	break;					/* returned in CX    	*/
						/*and 0F0F in AX for 	*/
						/* detected TSR */
case 0xff01:if(r.r_cx==HiPass|| r.r_dx==LoPass) /* FF01H - Settings User_Id 	*/
			{		        /* Jesli wywolywacz podal	*/
			r.r_ax=0x0;             /* prawidlowa wartosc Pass	*/
			User_Id=r.r_bx;		/* To za Id wstawia wartosc z bx*/
			}
			else			/* Jesli nie to zwraca blad 5	*/
			{
			r.r_ax=5;		/* Kod bledu do rejestru powrotnego */
			r.r_flags|=CARRY;       /* I flaga bledu  ustawione ! 	    */
			} break;
case 0x4301:
	/* Zmiana atrybutow zabroniona !!! */
	{
	r.r_ax=5;/* Kod bledu do rejestru powrotnego */
	r.r_flags|=CARRY;
	}break;
}
return 0;
}
