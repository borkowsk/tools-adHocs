/*
Worspace User Interface - implementacja typu  paletowego dla VGA
*/

#ifdef __MSDOS__
// On VGA only 6 high bits is significant
#define SHIFT 2
#else
#define SHIFT 0
#endif

#include <stdlib.h>
#include <string.h>
#include "biosgraf.h"
#include "picture.hpp"

typedef
unsigned char byte;
typedef
unsigned int  word;


inline long int sqr(int w)
{
return w*w;
}

inline void palette::Mask(RGB& w)
{
w.R>>=SHIFT;w.G>>=SHIFT;w.B>>=SHIFT;
}

inline void palette::UnMask(RGB& w)
{
w.R<<=SHIFT;w.G<<=SHIFT;w.B<<=SHIFT;
}

palette::palette(unsigned LN,unsigned LS)  /* Konstruktor palety o N kolorach, startujacy os S */
{
if(LS>255)
	LS=0;
if(LS+LN>256)
	LN=256-LS;
nodes=new RGB[LN];
if(nodes==NULL)
	goto ERROR;
locks=new byte[LN];
if(locks==NULL)
	goto ERROR;
N=LN;
S=LS;
cubes=cubea=0;
flag=1;
not_lock=256;
if(screen::IsInGraph())
		fromhardware();
goto EXIT;
ERROR:
if(nodes!=NULL)
	delete nodes;
N=0;S=0;
EXIT: ;
}

palette::~palette()
{
delete nodes;
delete locks;
N=S=cubea=cubes;
}

void palette::Lock(color_handle c_h)    /* Zablokowanie koloru przed zmianami */
{
if(c_h>=S && c_h<S+N)
	if(locks[c_h-S]<255)
		{
		locks[c_h-S]++;
		not_lock--;
		}
}

void palette::Unlock(color_handle c_h)  /* Odblokowanie koloru */
{
if(c_h>=S && c_h<S+N)
     if(locks[c_h-S]>0)
	{
	locks[c_h-S]--;
	not_lock++;
	}
}

void palette::Free(color_handle c_h)    /* To samo co Unlock - do pary z Alloc   */
{
if(c_h>=S && c_h<S+N)
   if(locks[c_h-S]>0)
	{
	locks[c_h-S]--;
	not_lock++;
	}
}

unsigned palette::ColorDist(RGB& A,RGB& B)  /* Odleglosc miedzy kolorami */
{
//return 30*abs(A.R-B.R)+59*abs(A.G-B.G)+11*abs(A.B-B.B);
//return isqrt(sqr(A.R-B.R)+sqr(A.G-B.G)+sqr(A.B-B.B));
return abs(A.R-B.R)+abs(A.G-B.G)+abs(A.B-B.B);
}

RGB  palette::Values(color_handle h)  /* Wartosci RGB zwiazane z uchwytem koloru */
{
if(h>=S && h<S+N)
	{
	RGB pom=nodes[h-S];
	UnMask(pom);
	return pom;
	}
return RGB(0,0,0);
}

color_handle palette::Alloc( RGB rgb,unsigned th) /* Uchwyt koloru nie dalszego niz th od zadanego lub 0 (brak miejsca) */
{
unsigned i=(S!=0?0:1),freepos=0xffff,pom=0,min=0xffff,dist=0;
Mask(rgb);
do{
dist=ColorDist(rgb,nodes[i]);
if(dist<=th) // Znaleziono zgodny z wymaganiami
	{min=dist;pom=i;break;}
if(dist<min)
	{min=dist;pom=i;}
if(locks[i]==0)
	{freepos=i;}
i++;
}while(i<N);
if(min>th && freepos!=0xffff) // Istniejace nie spelniaja wymagan, a jest jeszcze miejsce
	{
	pom=freepos;
	nodes[pom]=rgb;
	if(flag)
		tohardware(pom);
	}
locks[pom]++;
not_lock--;
return pom+S;
}

color_handle palette::FromCube( RGB)  /* Uchwyt koloru z kostki kolorow */
{
return 0;
}

void palette::cube(int n)    /* Wpisanie na koncu tablicy RBG kostki kolorow */
{
size_t size=n*n*n,i,j,k;
size_t pom=N-size;
size_t skok=255/n;
if(size>N)
	return;
for(i=1;i<=n;i++)
	for(j=1;j<=n;j++)
		for(k=1;k<=n;k++)
		{
		pom++;
		RGB rgb(i*skok,j*skok,k*skok);
		Mask(rgb);
		nodes[pom]=rgb;
		locks[pom]++;
		}
not_lock-=size;
}


void palette::grayscale(int n)    /* Wpisanie na koncu tablicy skali szarosci*/
{
size_t size=n,i;
size_t pom=N-size;
size_t skok=256/n;
if(size>N)
	return;
for(i=0;i<n;i++)
	{
	pom++;
	RGB rgb(i*skok,i*skok,i*skok);
	Mask(rgb);
	nodes[pom]=rgb;
        locks[pom]++;
        }
not_lock-=size;
}
