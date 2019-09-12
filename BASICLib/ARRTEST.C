#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/* For heap errors debuging */
#define	  _HEAP_TESTING_

#include "basic.hpp"	    /* Included always ! */
#include "onerror.hpp" 	    /* Included in  main file only  !!! */

#include "basstring.hpp"
#include "randfile.hpp"


#define _MATRIX_DEFINITION_ /*  Non inline matrix metod
				included only in this file */
//#define _LARGE_TABLE_      /*  Matrix indexes to 0xFFFF in small data models */
#define _INDEX_MODE_ 0      /*  Array and matrix indexed from 0 */


#define _TYPE_ int
#define _INITIALIZE_ 0
#include "matrix.hpp"

int_matrix AM;
int BS[12+_INDEX_MODE_][15+_INDEX_MODE_];
int_matrix aliasBS(BS,sizeof(BS),15+_INDEX_MODE_);

#define _TYPE_ char
#define _INITIALIZE_ 0
#include "array.hpp"
char_array P;
char ZZ[16+_INDEX_MODE_];
char_array aliasZZ(ZZ,sizeof(ZZ));

#define _TYPE_ int
#define _INITIALIZE_ 0
#include "array.hpp"
int_array A,B,C;

#define _TYPE_ double
#define _INITIALIZE_ 0
#include "array.hpp"
double_array X,Y,Z;

typedef long int longint;
#define _TYPE_ longint
#define _INITIALIZE_ 0
#include "array.hpp"
longint_array K,L,M;

#define _TYPE_ bstring
#include "array.hpp"
bstring_array S;

void main()
{
bstring bb="!";
char i=0;
 DEF_SEG(0xB800);// Segment pami‘ci ekranu karty EGA i VGA w trybie znakowym
     POKE(0x10,0x0F01);//
     clrscr();
     i=PEEK(0x10);// c typu znakowego zawiera kod 32 ježli czyszczenie ekranu
		  // zosta’o dokonane
printf("Bounds aliasZZ: %u-%u \n",aliasZZ.lbound(),aliasZZ.ubound());
printf("Bounds aliasBS: %u-%u %u-%u \n",aliasBS.lbound(1),aliasBS.ubound(1),aliasBS.lbound(2),aliasBS.ubound(2));
P.dim(16);
AM.dim(10,10);
printf("Bounds P: %u-%u \n",P.lbound(),P.ubound());
printf("Bounds AM: %u-%u %u-%u \n",AM.lbound(1),AM.ubound(1),AM.lbound(2),AM.ubound(2));
ZZ[0]='+';
ZZ[3]='@';
AM[1][1]=11;

AM(1,1)=11;
AM(2,2)=22;
AM(2,3)=23;
AM(3,2)=32;
AM[4][3]=43;
AM[3][4]=34;
AM(10,10)=1;
aliasZZ[3]='$';
aliasZZ(3)='%';
aliasZZ(0XF)='!';
aliasBS(1,1)=11;
aliasBS(2,2)=22;
aliasBS(2,3)=23;
aliasBS(3,2)=32;
aliasBS[4][3]=43;
aliasBS[3][4]=34;
aliasBS(10,10)=1;

//aliasBS(11,11)=1;//Always RangE !
A.dim(10);
AM.dim(10,10);
A(1)=10;
//#if _INDEX_MODE_==1
//A(10)=1;
//#else
A(9)=1;
A(1)=0;
//#endif
K.dim(20);
K(1)=20;
K(20)=1;// if _INDEX_MODE_ == 0 - error !!!
X.dim(30);
X(1)=30;
X(30)=1;
P.dim(40);
P(1)=40;
P(40)=1;
S.dim(10);
S(1)="Q"+bb;
S(5)="SSSSS"+bb;
S(10)="ZZZZZZZZZZ"+bb;
if(S[1]==S[5])
	printf("Strange error in comparision !\n");
if(!(S[2]==S[3]))
	printf("Strange error in second comparision !\n");
S.erase();
S.redim(20);
//aliasZZ(0)='-';// Rang error if _INDEX_MODE_==1
//B(0)=1;// Not alocated - error !!!
}

