
/* NISKOPOZIOMOWE FUNKCJE BIOSOWEGO WYPROWADZANIA ZNAKOW */

/* Poniewaz przerwanie 13H niszczy rejestry SI,DI,BP
trzeba je zabezpieczac w funkcjach inline. Nie mozna uzyc
wstawek assemblerowych w funkcji inline stad dwie ponizsze funkje */

static unsigned int SSI,SDI,SBP;
inline void savereg()
{
SDI=_DI;
SSI=_SI;
SBP=_BP;
}

inline void restreg()
{
_DI=SDI;
_SI=SSI;
_BP=SBP;
}

static /*inline*/ void BiosTTYChar( unsigned char c)
{
savereg();
_BH=PAGE;
_BL=COLOR;
_AH=0x0e;
_AL=c;
geninterrupt(0x10);
restreg();
}

static /*inline*/void BiosAChar( unsigned char c,unsigned char a)
{
savereg();
_BH=PAGE;
_CX=1;
_BL=a;
_AL=c;
_AH=0x9;
geninterrupt(0x10);
restreg();
}

static /*inline*/void BiosChar( unsigned char c)
{
savereg();
_BH=PAGE;
_CX=1;
_AL=c;
_AH=0xA;
geninterrupt(0x10);
restreg();
}

static /*inline*/ void BiosGotoXY(unsigned int x,unsigned int y)
{
savereg();
_BH=PAGE; //strona aktywna
_DH=y;
_DL=x;
_AH=0x2;
geninterrupt(0x10);
restreg();
}
