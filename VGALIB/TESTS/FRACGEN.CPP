/*
genetyka fraktali
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <values.h>
#include <math.h>
#include <conio.h>

#include "biosgraf.h"
#include "picture.hpp"
#include "screens.hpp"
#include "fonts.hpp"
#include "window.hpp"

#define BREAK if(kbhit()){getch();break;}

void demonstration(picture& S);

main()
{
picture* Z=screen::StartMenu();
picture &S=*Z;
//Window W(S,10,10,S.GetMaxX()/2,S.GetMaxY()/2);
randomize();
S.Clear();
demonstration(S);
delete Z;
return 0;
}


class Afinity
{
unsigned IL_PRZEKSZTALCEN;
unsigned long IL_ITERACJI;// liczba iteracji rysunku
double* A; // kolejne wspolczynniki przeksztalcenafinicznych...
double* B;
double* C;
double* D;
double* E;
double* F;
picture* target;//Wirtualny ekran wyprowadzania
double a,b,c,d; // wspolczynniki skalowania
public:
Afinity(unsigned long iteraci,size_t przekszt,double* tab);// konstruktor
void draw();     // rysowanie
void SetViewPort(picture& t,double X1,double Y1,double X2,double Y2);// Ustalanie okna wyprowadzania
void SetNumber(unsigned n) { IL_PRZEKSZTALCEN=n;}
};

void demonstration(picture& S)
{
/* Trojkaty */
extern double TrojkatyTab[];
/* Obrzeza trojkatow */
extern double ObrzerzaTab[];
/* Drzewko - psylofit */
extern double PsilofitTab[];
/* Drzewko - paproc */
extern double PaprocTab[];
/* Cos ? */
extern double CostamTab[];
/* Galaktyka ? */
extern double SpiralaTab[];
/* Wariacja */
extern double WariacjaTab[];
Afinity Trojkaty(10000,3,TrojkatyTab);
Afinity Obrzerza(10000,3,ObrzerzaTab);
Afinity Psilofit(10000,4,PsilofitTab);
Afinity Paproc(20000,4,PaprocTab);
Afinity Costam(10000,3,CostamTab);
Afinity Spirala(10000,3,SpiralaTab);
Afinity Wariacja(10000,3,WariacjaTab);
Afinity* A=NULL;
float X1,Y1,X2,Y2;
unsigned wybor;
do{
JESZCZERAZ:
gotoxy(1,3);//DOS conio !!!
printf("Rodzaj figury:");
printf(" 1: Trojkaty\n"
	  "2: Obrzerza\n"
	  " 3: Psilofit\n"
	  " 4: Paproc\n"
	  " 5: Costam\n"
	  " 6: Spirala\n"
	  " 7: Wariacja\n");
scanf("%d",&wybor);
switch(wybor){
	  case 1:A= &Trojkaty;break;
	  case 2:A= &Obrzerza;break;
	  case 3:A= &Psilofit;break;
	  case 4:A= &Paproc;break;
	  case 5:A= &Costam;break;
	  case 6:A= &Spirala;break;
	  case 7:A= &Wariacja;break;
	  default:S<<"B L A D !!!";goto JESZCZERAZ;
          };
printf("Gorny rog. \nX:");
scanf("%f",&X1);
printf("Y:");
scanf("%f",&Y1);
printf("Dolny rog. \nX:");
scanf("%f",&X2);
printf("Y:");
scanf("%f",&Y2);
S.Clear();
A->SetViewPort(S,X1,Y1,X2,Y2);
A->draw();
S.SetColor(1);
S.OutTextXY(1,1,"q-exit ESC=new draw");
}while(getch()!='q');
}

Afinity::Afinity(unsigned long iteraci,size_t przekszt,double* tab)
{
IL_PRZEKSZTALCEN=przekszt;
IL_ITERACJI=iteraci;
A=tab;
B=tab+IL_PRZEKSZTALCEN;
C=tab+2*IL_PRZEKSZTALCEN;
D=tab+3*IL_PRZEKSZTALCEN;
E=tab+4*IL_PRZEKSZTALCEN;
F=tab+5*IL_PRZEKSZTALCEN;
}

void Afinity::SetViewPort(picture& t,double X1,double Y1,double X2,double Y2)
// Ustalanie okna wyprowadzania
{
if(X2==X1 || Y1==Y2)
         {
         X2=X1=Y2=Y1=1;
         return ;
         }
target=&t;
a=target->GetMaxX()/(X2-X1);
b=-a*X1;
c=target->GetMaxY()/(Y2-Y1);
d=-c*Y1;
}

void Afinity::draw()
{
double x1=0,x=0,y=0;
unsigned i;
target->PutPixel(a*0+b,c*0+d,1);
for(i=1;i<=IL_ITERACJI;i++)
   {
   unsigned NR_PRZEKSZTALCENIA=random(IL_PRZEKSZTALCEN);
   x1=A[NR_PRZEKSZTALCENIA] * x +
      B[NR_PRZEKSZTALCENIA] * y +
      E[NR_PRZEKSZTALCENIA];
    y=C[NR_PRZEKSZTALCENIA] * x +
      D[NR_PRZEKSZTALCENIA] * y +
      F[NR_PRZEKSZTALCENIA];
    x=x1;
   target->PutPixel(a*x+b,c*y+d,15/*NR_PRZEKSZTALCENIA*/);
   }
}