/* Program odczytuje Blok DPB dla wyspecyfikowanego jako parametr dysku */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <process.h>
#include <dos.h>
#include <ctype.h>

struct DPB {
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

void printDPB(DPB far* b)
{
printf(" Numer dysku : %u \n",(unsigned char) b->DiscNum);
printf(" Numer jednostki w programie obslugi : %u \n",(unsigned char) b->InObsNum);
printf(" Wielkosc sektora w bajtach : %u \n",(unsigned int) b->SecSize);
printf(" Liczba sektorow w JAP-1 : %u \n",(unsigned char) b->SecJAP_1);
printf(" log2 z liczby sektorow w JAP : %u \n",(unsigned char) b->logSecJAP);
printf(" Liczba sektorow zarezarwowanych przed FAT-em : %u \n",(unsigned int) b->SecReserved);
printf(" Liczba kopi FAT : %u \n",(unsigned char) b->FATnum);
printf(" Maksymalna liczba plikow w katalogu glownym : %u \n",(unsigned int) b->FileMax);
printf(" Pierwszy sektor danych : %u \n",(unsigned int) b->FirstData);
printf(" Najwiekszy numer JAP - liczba JAP przeznaczonych na dane +1: %u \n",(unsigned int) b->MaxJAP);
printf(" Liczba sektorow zajetych przez tablice FAT : %u \n",(unsigned int ) b->FATSize);
printf(" Numer pierwszego sektora katalogu : %u \n",(unsigned int ) b->RootFirst);
printf(" Wskaznik do programu obslugi dysku : %Fp \n",(void far * ) b->Procedure);
printf(" Bajt identyfikacji nosnika : %u \n",(unsigned char) b->MediaBayt);
printf(" Bajt dostepu do dysku - 0xFF - nie zadano : %u \n",(unsigned char) b->AccesBayt);
printf(" Wskaznik do nastepnego DPB - 0xffff -ostatni : %Fp \n",(DPB far*     ) b->Follow);
printf(" Pierwsza wolna JAP na dysku : %u \n",(unsigned int  ) b->FistUnUse);
printf(" Liczba wolnych JAP na dysku - 0xffff - nie znana : %u \n",(unsigned int) b->UnUsedJAP);
}

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

void main()
{
DPB far* thisDPB;
unsigned char Disk=3;
if(_argc>=2)
	Disk=toupper(_argv[1][0])-'@';
	else
	Disk=0;
printf("\n\tB L O K   P A R A M E T R O W   DYSKU    %c: \n",64+Disk);
thisDPB=GetDPB(Disk);
if(thisDPB==NULL){
	   struct DOSERROR info;
	   dosexterr(&info);
	   printf("Extended DOS error information:\n");
	   printf("   Extended error:     %d\n",info.de_exterror);
	   printf("            Class:     %x\n",info.de_class);
	   printf("           Action:     %x\n",info.de_action);
	   printf("      Error Locus:     %x\n",info.de_locus);
	   exit(1);
	}
printDPB(thisDPB);
}