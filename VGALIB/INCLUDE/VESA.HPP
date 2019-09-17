/*
 Funkcje VESA bios extension
*/

#ifdef __cplusplus
extern "C" {
#endif

extern char ISVESA;
extern unsigned char far *WINA;
extern unsigned char far *WINB;

extern unsigned char ATR_A;
extern unsigned char ATR_B;
extern unsigned char BANK_NUM;// ile jest bankow
extern unsigned long int GRANULARITY;//co ile B zaczyna sie okno
extern unsigned int MULTIBANK;// o ile trzeba mnozyc dla okien co 64KB
extern unsigned long int  WIN_SIZE;// rozmiar okna w pamieci systemu
extern void (far* ChangeBank)();// funkcja przelanczajaca banki

struct VESAinfo
	{
	char Syg[4];
	unsigned Nr;
	char far* NazwaProd;
	long int Inform;
	unsigned  far* ListaTr;
	char RESERVED[238];
	} ;

struct MODEinfo
	{
	unsigned int A_mode;
	unsigned char A_win_A;
	unsigned char A_win_B;
        unsigned int Granularity;
	unsigned int WindSize;
	unsigned int SegA;
	unsigned int SegB;
	void (far* SwithProc)();
	unsigned int LineBytes;
	unsigned int Xsize;
	unsigned int Ysize;
	unsigned char CharHoriz;
	unsigned char CharVert;
	unsigned char Planes;
	unsigned char BitsPoint;
	unsigned char BankNum;
	unsigned char Model;
	unsigned char BankSizeP;
	unsigned char Reserved[227];
	};

int far IsVesa(VESAinfo far* buf);
void far VesaSetParameters();
int far VesaScreen(unsigned mode);
extern unsigned int BANKINA;

void inline AssignBank(unsigned char Win,unsigned int Bank)
{
BANKINA=Bank;
Bank*=MULTIBANK;
_DX=Bank;
_BL=Win;
_BH=0x00;/* Podfunkcja kojarzenie */
//_AX=0x4F05;/* Funkcja "Banki" */
ChangeBank();
//geninterrupt(0x10);
}

extern unsigned int far* LISTATRYBOW;
extern unsigned long int LINELEN;

#ifdef __cplusplus
}; //extern "C" {
#endif
