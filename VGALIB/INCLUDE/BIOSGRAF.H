/*
Header BIOS-owej obslugi karty graficznej
*/
#ifndef _INC_BIOSGRAF_H
#define _INC_BIOSGRAF_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{ unsigned char one,two; } twochar;

extern unsigned char far* SCREEN;
extern unsigned char PAGE;
extern unsigned char NUMPAGE;
extern unsigned char far& BIOSMODE;
extern unsigned char DEVICE;
extern unsigned char GRAPH;
extern unsigned int MODE;
extern unsigned int MAXX;
extern unsigned int MAXY;
extern unsigned int XRATIO;
extern unsigned int YRATIO;
extern unsigned int RATIOXY;
extern unsigned int COLOR;
extern unsigned int TMAXX;
extern unsigned int TMAXY;
extern unsigned int CHARMATTRIX;
extern unsigned int NUMCOLOR;

extern char *cards[];

struct RGBrec {
	unsigned I  :3;/* intensywnosc I skladowej  	*/
	unsigned II :3;/* intensywnosc II skladowej     */
	unsigned III:2;/* intensywnosc III skladowej   	*/
	};

/* Mnozniki skalujace kolory */
#define MUL_I   9
#define MUL_II  9
#define MUL_III 21

/* TYPY PALET W WYWOLANIU ColorContinuum */
#define RG_B 0
#define BR_G 1
#define GB_R 2
/*
Funkcja biosu 10H niszczy rejestry DI i SI zawierajace zmienne rejestrowe
*/
#define  Int10H() asm{push di;push si;push bp;int 16 ;pop  bp; pop  si;  pop  di;}

/* NIEZBEDNE OPERACJE BIOSU VIDEO */

twochar BiosDevices();
/* rozpoznanie kart video */

int far  BiosScreen( unsigned mode );
/* Ustawienie trybu graficznego karty */
/* ustala SCREEN na prawidlowa wartosc ? */

void  PrintModeInfo();
/* Drukuje na ekran podstawowe parametry graficzne */

void EditParameters();
/* Interakcyjna edycja parametrow trybu */

void wait_for_frame();
/* czekanie na ramke obrazu */

void  far BiosPage( unsigned char LPAGE );
/* BIOSowe przelaczenie strony */

void  far BiosCleardevice();
/* Czyszczenie aktywnej strony */

void  far BiosSetPixel( long int x , long int y ,unsigned int color );
/* ustawienie pixela wywolaniem INT 10 funkcja 12 */

unsigned int  far BiosGetPixel( long int x , long int y );
/* odczytanie pixela wywolaniem INT 10 funkcja 13 */


void  far BiosSetPROB( unsigned char val );
/* ustawienie rejestru PROB karty TopVGA */

void  far BiosSetPROA( unsigned char val );
/* ustawienie rejestru PROA karty TopVGA */

/* OPERACJE ZWIAZANE Z PALETA KARTY VGA */
void far BiosGetVGAPalette(unsigned char far *pal);
/* Odczytanie numerow kolorow trybu 16 kolorowego */

void far BiosSetRGB(unsigned int n,unsigned  char R,unsigned  char G,unsigned  char B);
/* Ustalenie jednego rejestru przetwornika C/A */

void far BiosSetRGBBlock(unsigned int s,unsigned int n,void far* table);
/* Ustawienie bloku n rejestrow od rejestru s wg tablicy RGB table */

void far BiosGetRGBBlock(unsigned int s,unsigned int n,void far* table);
/* Pobranie bloku n rejestrow od rejestru s wg tablicy RGB table */

inline unsigned int GetColorNum(RGBrec pom)
{ return *(unsigned char*)&pom; }
/* konwersja rekordu RGB na numer koloru - trywialna */

int far ColorContinum(int);
/* ustalenie kolorow RGB odpowiednich do kodowania rekordem RGB */

int far FuncContinum();
/* skala do uwypuklanie wartosci funkcji - dla 16 kolorow zlota, dla 256 tecza */

/* FUNKCJE GRAFIKI SREDNIEGO POZIOMU REALIZOWANE ZA POMOCA BIOSU */
void  far BiosLine( long int x1 , long int y1 ,long int x2 , long int y2 );
/* narysowanie lini w dowolnym kierunku */

void  far BiosCircle(long int xxs,long int yys,long int bb);
/* narysowanie okregu  */

void  far BiosEllipse(long int xxs,long int yys,long int bb,long int aa);
/* Elipsa algorytmmem Bresenhama */

void  far BiosScrollBar(long int x1, long int y1,long int x2, long int y2,int lines);
/* scroling prostokatnego obszaru ekranu */

void  far BiosClearBar(long int x1, long int y1,long int x2, long int y2);
/* oczyszczenie prostokatnego obszaru ekranu */

void  far BiosFillBar(long int x1, long int y1,long int x2, long int y2);
/* wypelnienie prostokatnego obszaru ekranu */

void  far BiosFillCircle(long int xxs,long int yys,long int bb);
/* wypelnienie kolistego obszaru ekranu */

void  far BiosFillEllipse(long int xxs,long int yys,long int aa,long int bb);
/* wypelnienie elipsoidalnego obszaru ekranu */


unsigned int isqrt(unsigned long int par);
/* czesc calkowita pierwiastka kwadratowego */

void linecopy( void far* p,void far* b,unsigned len );
/* kopiowanie z pamieci do pamieci(ekranu) z pominieciem zer */

/* Dlugosc linii */
extern unsigned long int LINELEN;

#ifdef __cplusplus
}; //extern "C" 
#endif


#endif