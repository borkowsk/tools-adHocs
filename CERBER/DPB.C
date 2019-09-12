/* Program odczytuje Blok DPB dla wyspecyfikowanego jako parametr dysku */
struct DPB {
unsigned char DiscNum;/* Numer dysku */
unsigned char InObsNum;/* Numer jednostki w programie obslugi */
unsigned int  SecSize;/* Wielkosc sektora w bajtach */
unsigned char SecJAP_1;/* Liczba sektorow w JAP-1 */
unsigned char logSecJAP;/* log2 z liczby sektorow w JAP */
unsigned int  SecReserved;/* Liczba sektorow zarezarwowanych przed FAT-em */
unsigned char FATnum; /* Liczba kopi FAT */
unsigned int  FileMAx; /* Maksymalna liczba plikow w katalogu glownym */
unsigned int  FirstData; /*Pierwszy sektor danych */
unsigned int  MaxJAP; /* Najwiekszy numer JAP - liczba JAP przeznaczonych na dane +1*/
unsigned int  FATSize; /* Liczba sektorow zajertych przez tablice FAT */
unsigned int  RootFirst; /* Numer pierwszego sektora katalogu */
void far *    Procedure; /* Wskaznik do programu obslugi dysku */
unsigned char MediaBayt; /* Bajt identyfikacji nosnika */
unsigned char AccesBayt; /* Bajt dostepu do dysku - 0xFF - nie zadano */
DPB far*      Follow;    /* Wskaznik do nastepnego DPB - 0xffff -ostatni */
unsigned int  FistUnUse; /* Pierwsza wolna JAP na dysku */

