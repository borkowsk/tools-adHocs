#include "biosgraf.h"

typedef
unsigned char byte;
typedef
unsigned int  word;

struct RGBnode
{
byte R,G,B;/* Skladowe koloru */
};

class color
{
static
RGBnode nodes[256];/* tablica RBG dla 256 kolorow */
static
byte    locks[256];/* tablica blokowania dla 256 kolorow */
static
byte Flag;
static
byte   cubea;
static
byte   cubes;

byte handle;/* Numer koloru */

public:
static void fromhardware();/*Odczytanie rejestrow kolorow karty */
static void cube(int n);/* Wpisanie na koncu tablicy RBG kostki kolorow */
static void tohardware();/* Uzgodnienie tablicy kolorow karty z wirtualna */
static void direct(byte d)/* Ustalenie natychmiastowego uzgadniania(1) lub opoznionego */
	{Flag=d;}

color() {handle=0;};
~color(){free();}

operator int()
	{return handle;}
void free()
{ unlock();handle=0;}/* Wyzerowanie handlera koloru na przezroczysto */
void unlock()	     /* Zdjecie zabezpieczenia z koloru */
{if(handle>=32 && locks[handle]>0) locks[handle]--;}/* Kolory do 31 sa stale */
byte alloc(byte R,byte G,byte B);/* Alokacja koloru - zwraca 0 jesli brak miejsca*/
byte getnear(byte R,byte G,byte B);/* Ustalenie koloru o najblizszym RGB */
byte allocnear(byte R,byte G,byte B);/* Alokacja koloru -przy braku miejsca zwraca najblizszy wg RGB */
byte getoralloc(byte R,byte G,byte B);/* Zwraca najblizszy jesli jest - lub alokuje */
byte getcube(byte R,byte G,byte B);
friend
word ColorDist(byte i,byte R,byte G,byte B);
};





