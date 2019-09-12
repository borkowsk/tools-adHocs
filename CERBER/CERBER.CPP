/***NOTE:
   This is an interrupt service TSR system
   You can NOT compile this program with Test
   Stack Overflow turned on and get an
   executable file which will operate
   correctly.

   This program use current buffer pointer
   from DOS  LL structure.
   Its mayby incompatible in future version of system.
   This code is not reentrant and
   not compatible witch shared time system.
*/

#pragma inline
#if !defined(__TINY__)
#error Mozna kompilowac TYLKO w modelu TINY
#endif

// #include <conio.h> /* For DEBUG only */
#include <dos.h>

#define hi(_I_)  (((_I_)&0xFF00)>>8)
#define lo(_I_)  ((_I_)&0x00FF)

/* Struktury systemowe */
/*---------------------*/
struct ffblk {  /* DOS file control block structure. */
    char      ff_reserved[21];
    char      ff_attrib;
    unsigned  ff_ftime;
    unsigned  ff_fdate;
    long      ff_fsize;
    char      ff_name[13];
    };

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
	unsigned char DiscNum;	/* Numer dysku */
	unsigned char InObsNum;	/* Numer jednostki w programie obslugi */
	unsigned int  SecSize;	/* Wielkosc sektora w bajtach */
	unsigned char SecJAP_1;	/* Liczba sektorow w JAP-1 */
	unsigned char logSecJAP;/* log2 z liczby sektorow w JAP */
	unsigned int  SecReserved;/* Liczba sektorow zarezarwowanych przed FAT-em */
	unsigned char FATnum; 	/* Liczba kopi FAT */
	unsigned int  FileMax;	/* Maksymalna liczba plikow w katalogu glownym */
	unsigned int  FirstData;/*Pierwszy sektor danych */
	unsigned int  MaxJAP; 	/* Najwiekszy numer JAP - liczba JAP przeznaczonych na dane +1*/
	unsigned int  FATSize; 	/* Liczba sektorow zajetych przez tablice FAT */
	unsigned int  RootFirst;/* Numer pierwszego sektora katalogu */
	void far *    Procedure;/* Wskaznik do programu obslugi dysku */
	unsigned char MediaBayt;/* Bajt identyfikacji nosnika */
	unsigned char AccesBayt;/* Bajt dostepu do dysku - 0xFF - nie zadano */
	struct DPB far*  Follow;/* Wskaznik do nastepnego DPB - 0xffff -ostatni */
	unsigned int  FistUnUse;/* Pierwsza wolna JAP na dysku */
	unsigned int  UnUsedJAP;/* Liczba wolnych JAP na dysku - 0xffff - nie znana */
	};

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

#define NULL 0L
/* The clock tick interrupt */
#define INTR 0x1C
#define INTDOS 0x21
/* Screen attribute (blue on grey) */
#define ATTR 0x0F00
#define CARRY 0x01
#define NO_CARRY 0xFE
#define ATTR_MASK 0xE7

char far* LLpointer(); /* Pobiera adres struktury LL */
unsigned realDOSversion(); /* Pobiera rzeczywista wersje DOS-u */
int freeenviroment();
int Cerber21H(void);/* Funkcja modyfikujaca serwis systemowy */
void interrupt DOShandler();
void interrupt handler();

/* Przekazanie sterowania pod adres orginalny przerwania*/
#define ChainIntr(P) asm{pop bp;     \
			 pop di;     \
			 pop si;     \
			 pop ds;     \
			 pop es;     \
			 pop dx;     \
			 pop cx;     \
			 pop bx;     \
			 pop ax;     \
			 jmp cs:[P]; /* Tylko model TINY !!! */\
			 }

/* reduce heaplength and stacklength
to make a smaller program in memory */
extern unsigned 	_heaplen = 1;/* Nie potrzebna ?*/
extern unsigned 	_stklen  = 1024;
void 	interrupt 	( *OrginalTimerHandle)()=NULL;
void 	interrupt 	( *OrginalDOS21h)()=NULL;
struct 	LL 	far* LL=NULL;
struct 	LLst 	far* LLst=NULL;
struct 	BUF 	far* far* curbuf=NULL;
unsigned 	char far* InDos=0;
unsigned 	int error=0;
unsigned 	int MySS=0;
unsigned 	int MySP=0;
unsigned 	int CallerSS=0;
unsigned 	int CallerSP=0;
unsigned        char User_Id=0;/* Identyfikator aktualnie pracujacego uzytkownika */
unsigned int 	(far *screen)[80];

/* STRUKTURA DO PRZECHOWYWANIA REJESTROW WYWOLANIA PO ZMIANIE STOSOW */
struct REGPACK r;

/* PRZYGOTOWANIE DO PRACY I INSTALACJA TSR-a */
int main(void)
{
/* Save actual top of stack */
MySS=_SS;
MySP=_SP;

/* Get struct LL pointer */
LL=(struct LL far*)LLpointer();
LLst=(struct LLst far* )(LLpointer()-0x0C);
curbuf=&(LLst->curbuf); /* Current bufer pointer */

/* For a color screen the video memory  is at B800:0000.
   For a monochrome system use B000:000 */
_AH=0x0F;
geninterrupt(0x10);
if( _AL !=7)
(void far* )screen = MK_FP(0xB800,0);
else
(void far* )screen = MK_FP(0xB000,0);

/* Read InDos system variable addres */
_AH=0x34;geninterrupt(0x21);
InDos=MK_FP(_ES,_BX);

/* get the addreses of interrups */
OrginalTimerHandle = getvect(INTR);
OrginalDOS21h = getvect(INTDOS);

/* Free environ segment */
if(freeenviroment())
	goto blad;

/* install the new interrupt handler */
setvect(INTDOS, DOShandler);

/*
	Terminate and stay resident
-----------------------------------------------
   _psp is the starting address of the
   program in memory.  The top of the stack
   is the end of the program.  Using _SS and
   _SP together we can get the end of the
   stack.  You may want to allow a bit of
   saftey space to insure that enough room
   is being allocated ie:
   (_SS + ((_SP + safety space)/16) - _psp)
*/
keep(0, (MySS + ((MySP+16)/16) - _psp));/* Juz nie wraca z tego wywolania */

/* Not OK - Not instal ! */
blad:
return 1;
}

/* FUNKCJA OBSLUGUJACA PRZERWANIE DOSU 21H */
#define U unsigned
void interrupt DOShandler(U bp,U di,U si,U ds,U es,U dx,U cx,U bx,U ax,U ip,U cs,U flags)
#undef U
{
unsigned oldscreen;
/*
if(ax>0x0D00)  */
  if(ax==0x4301 || ax>=0xff00)
/*
	   ax==0x1300 || /* skasowanie pliku */
	   ax==0x3A00 || /* usuniecie  podkatalogu */
	   ax==0x4100 || /* skasowanie   pliku */
	   ax==0x5600 || /* zmiana nazwy   pliku */
*/
	{
	/* Zapamientanie rejestrow w struktorze REGPACK*/
	r.r_ax=ax;
	r.r_bx=bx;
	r.r_cx=cx;
	r.r_dx=dx;
	r.r_si=si;
	r.r_di=di;
	r.r_ds=ds;
	r.r_es=es;
	r.r_flags=flags;
	r.r_bp=bp;

	(*InDos)++;
	r.r_flags&=NO_CARRY;
	oldscreen=screen[0][79];
	screen[0][79] ='²' + ATTR;
	disable();
	CallerSS=_SS;
	CallerSP=_SP;
	/* Przelanczanie stosu do stanu z MAIN */
	_SS=MySS;
	_SP=MySP;
	enable();
	/* Funkcja obslugujaca z casem w srodku */
	Cerber21H();
	/* Przywrocenie rejestrow stosu */
	disable();
	_SP=CallerSP;
	_SS=CallerSS;
	enable();
	if(screen[0][79]=='²' + ATTR) /* Przywracamy ekran jesli sie ni zamazalo */
		screen[0][79] =oldscreen;
	(*InDos)--;

	/* Odtworzenie rejestrow na stosie by funkcje mogly cos przekazac*/
	ax=r.r_ax;
	bx=r.r_bx;
	cx=r.r_cx;
	dx=r.r_dx;
	si=r.r_si;
	di=r.r_di;
	ds=r.r_ds;
	es=r.r_es;
	flags=r.r_flags;
	bp=r.r_bp;

	goto END;
	}
ChainIntr(OrginalDOS21h);
END:
;
}

/* BLOK DEFINICJI FUNKCJI IMPLEMENTUJACYCH DZIALANIA TSR-a */
int freeenviroment()
{
/* Zwolnienie bloku srodowiska */
/* Czasami moze powodowac uszkodzenie lancucha blokow !? */
unsigned int environ_seg=*((unsigned int far*)MK_FP(_psp,0x002C));/* Adres segmentu srodowiska */
asm push es;
_ES=environ_seg;		/* Wpisanie tej wartosci do _ES */
_AH=0x49;
geninterrupt(0x21);
asm pop es;
asm jc blad;
return 0;
blad:
return 1;
}

char far* LLpointer()
/* Uzywana przed zaistalowaniem */
{
asm mov bx,0;// _BX=0;
asm mov ah,52H;
asm int 21H; // Change es & bx
return  (char far*)MK_FP(_ES,_BX);
}

/*
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
*/

/* Funkcja  pobierania bloku parametrow dysku */
/*-------------------------------------------*/
struct DPB far* GetDPB(unsigned char D)
{
struct DPB far* retval;
unsigned char blad;

_DL=D;
_AH=0x32;
asm push ds;/* Save DATA segment */
OrginalDOS21h();
blad=_AL;
retval=(struct DPB far*)MK_FP(_DS,_BX);
asm pop ds;/* Restore DATA segment */

if(blad!=0) return NULL;
   else   return retval;
}

unsigned Chmod(void far* p,unsigned char a,unsigned char attr)
{
if(a==1)
	{
	_CH=0;
	_CL=attr;
	}
asm push ds;
_AH=0x43;
_AL=a;
_DX=FP_OFF(p);
_DS=FP_SEG(p);
asm pushf
asm call cs:[OrginalDOS21h];/* Bo DS moze miec inna wartosc ! */
asm jc ERROR;
asm pop ds;/* Restore DATA segment */
return _CX;
ERROR:
asm pop ds;/* Restore DATA segment */
error=_AX;
return 0xffff;
}

void SetDTA(void far* NewDTA)
{
asm push ds;
_DX=FP_OFF(NewDTA);
_DS=FP_SEG(NewDTA);
_AH=0x1A;
asm pushf
asm call cs:[OrginalDOS21h];/* Bo DS moze miec inna wartosc ! */
asm pop ds;
}

void far* GetDTA(void)
{
_AH=0x2F;
OrginalDOS21h();
return MK_FP(_ES,_BX);
}

int FindFirst(char far * path,void far* LocalDTA,unsigned char attr)
{
unsigned int ecode,flag;
void far* OldDTA=GetDTA();/* Bufor transmisji wywolujacego */
SetDTA(LocalDTA);	  /* Bufor tramsmisji TSR-a */

asm push ds;
asm mov cx,attr;
_DX=FP_OFF(path);
_DS=FP_SEG(path);
asm mov ah,0x4E;
asm mov al,0;
asm pushf
asm call cs:[OrginalDOS21h];/* Bo DS juz ma inna wartosc ! */
asm pop ds;
ecode=_AX;
flag=_FLAGS;

SetDTA(OldDTA);		/* Z powrotem bufor transmisji wywolujacego */
if( flag & CARRY )
	{ error=ecode;return -1; }/* Kod bledu zwrocony programowi wywolujacemu */
	else return 0;/*OK ! opis w obszarze DTA .*/
}

struct dir_entry far* pass_to_entry(char far* path)
/* Funkcja sprawdza czy aktualny "user" ma prawo grzebac w tym pliku */
/* Jesli tak to zwraca wskaznik do pozycji w buforze - jesli nie to NULL */
{
struct dir_entry far* dent=NULL;
  /* Szukanie pozycji katalogu */
char myDTA[sizeof(struct ffblk)];

if(FindFirst(path,myDTA,0xFF)==0) /* OK -opis w myDTA */
   {
    struct ffprivate* ffp=(struct ffprivate* )myDTA;
    unsigned int posit=ffp->posit%16;	/* Pozycja w aktualnym sektorze katalogu */
    if( Chmod(path,0,0)==0xffff)/* Path is full - force to load sector */
		goto ERROR;
    dent=((struct dir_entry far*)((*curbuf)->val)+posit); /* Obliczenie adresu w buforze */

//  DEBUGING PRINT
//-------------------
//    textcolor(LIGHTRED);
//    gotoxy(60,2);clreol();cprintf("Uid %u F_owner %u",User_Id,dent->user_id);
//    textcolor(WHITE);
//    putmem(dent,2);

    if(!(            /* D O S T E P N Y   D L A */
       User_Id==1	||          /* SUPERUSER */
       dent->user_id==0	||	    /* jest ogolnie dostepny */
       dent->user_id==User_Id       /* ten user */
       ) )
       goto NO_PASS;    /* TO NIE WOLNO ! */
   return dent;	/* WOLNO ! */
   }
   else goto ERROR;

/* Zmiana danych w uchwycie zabroniona !!! */
NO_PASS:		/* Kod bledu do rejestru powrotnego struktury r */
// cprintf("<%u %Fs>",dent->user_id,dent->name);
r.r_ax=5 ;
r.r_flags|=CARRY;
return NULL;
ERROR:			/* Wartosci bledow do  struktory rejestrowej r - zwracane do wywolujacego */
r.r_ax=error;
r.r_flags|=CARRY;	/* Bledy systemowe uniemozliwiajace odczyt lub zapis */
return NULL;
}

/* Glowna funkcja sterujaca praca TSR-a */
int Cerber21H()
{
unsigned  retchmod;
error=0;

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
case 0xff02:					/* FF02H - Set file owner */
	{
	struct dir_entry far* this=NULL;
	if( (this=pass_to_entry(MK_FP(r.r_ds,r.r_dx)))!=NULL)/* Odszukanie pozycji wg nazwy */
		/* Zmiana user'a DOZWOLONA wiec zmieniamy */
		{
		struct f_attrib pom=this->attr;
		this->user_id=r.r_cx;
		retchmod=*(unsigned char*)&pom;
		retchmod&=ATTR_MASK;
		/* Zmieniono wiec wymaga zapisu */
		if( retchmod!=Chmod( MK_FP(r.r_ds,r.r_dx),1, retchmod ) )/* Force write sector */
				{
			    //  sound(100);
			  //	cprintf("[%x %Fs] ",retchmod,MK_FP(r.r_ds,r.r_dx));
			   //	delay(200);
			   //	nosound();
				r.r_ax=error;
				r.r_flags|=CARRY;
				goto END;
				}
		r.r_ax=0;
		r.r_flags&=NO_CARRY;
		}
	}break;
case 0x4301:
	{
	struct dir_entry far* this=NULL;

	if( (this=pass_to_entry(MK_FP(r.r_ds,r.r_dx)))!=NULL)/* Odszukanie pozycji wg nazwy */
		/* Zmiana atrybutow DOZWOLONA wiec zmieniamy */
		{
		/* Nie mozna nadac attr. katalogu ani etykiety */
		struct f_attrib pom;
		asm{ mov ax,r.r_cx;
		     and ax,ATTR_MASK;
		     mov pom,ax; }
		this->user_id=User_Id;
		/* Zmiana atrybutu wymusza tez zapis */
		if( (retchmod=Chmod(MK_FP(r.r_ds,r.r_dx),1,*(unsigned char*)&pom))==0xffff )/* Force write sector */
				{
			  //	sound(100);
			  //	gotoxy(1,1);
			  //	cprintf("(%u %Fs)   ",retchmod,MK_FP(r.r_ds,r.r_dx));
			  //	delay(200);
			 //	nosound();
				r.r_ax=error;
				r.r_flags|=CARRY;
				goto END;
				}
		r.r_ax=0;
		r.r_flags&=NO_CARRY;
		}
	}break;
}
END:
return 0;
}

