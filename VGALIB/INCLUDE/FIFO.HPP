#include <stdlib.h>
extern void error(char*,int);

template<class itemtype>
class fifo
{
public:
fifo(size_t t_size)
//kolejka o okreslonej dlugosci
            {
            size=t_size;
            if(size>0)
                      items=new itemtype[size];
            putindex=getindex=notempty=0;
            }

void clean()
// czyszczenie pol - uniemozliwia zwolnienie pamieci
     {
     size=0;
     putindex=getindex=notempty=0;
     items=0;
     }

~fifo()
// Usuwanie kolejki z pamieci
       {
       if(size>0) delete []items;
       clean();
       }

int put(itemtype& toput)
// wstawianie do kolejki
             {
             if(notempty<size)
               {
               items[putindex]=toput;// wstawione
               putindex++;    //powiekrzenie indeksu
               putindex%=size;//ograniczenie do rozmiaru tablicy
               notempty++;    //powiekszenie licznika zawartosci
               }
               else return 0;
             return 1;
             }

int get(itemtype& toget)// wyjmowanie z kolejki
         {
         if(notempty>0)
               {
               toget=items[getindex];// pobranie
               getindex++;    //powiekrzenie indeksu
               getindex%=size;//ograniczenie do rozmiaru tablicy
               notempty--;    //zmniejszenie licznika zawartosci
               }
               else return 0;
         return 1;
         }

fifo& operator << (itemtype tp)
//operator wstawiania z wbudowana obsluga blendow
      {
      if(!put(tp)) error("FIFO OVERFLOW",1);
      return *this;
      }

fifo& operator >> (itemtype& tg)
//operator pobierania z wbudowana obsluga blendow
      {
      if(!get(tg)) error("FIFO UNDERFLOW",0);
      return *this;
      }

protected:
size_t putindex;//pozycja dopisywania
size_t getindex;//pozycja odczytywania
size_t notempty;//ilosc wpisanych i nie odczytanych pozycji
size_t size;    //aktualna pojemnosc kolejki
itemtype* items;//wskaznik do tablicy elementow kolejki
};

template<class itemtype>
class exfifo:public fifo<itemtype>
{
public:
exfifo(size_t t_basesize):fifo<itemtype>(t_basesize)
// kolejka o powiekszalnej pojemnosci
              {
              basesize=t_basesize;
              }

~exfifo()
// likwidacja
         {
         basesize=0;
         }

int expand()
// powiekszanie pojemnosci kolejki
     {
     itemtype* dump=items; // zapamientanie wskaznika do tablicy elementow
     size_t oldsize=size;  // i jej rozmiaru
     items=new itemtype[size+basesize];//alokacja nowej tablicy wiekszej o jednostke
     if(items==NULL) return 0;// Wyjscie gdy  nieobsluzony brak pamieci
     memcpy(items,dump+getindex,(oldsize-getindex)*sizeof(itemtype));//inteligentne
     memcpy(items+(oldsize-getindex),dump,getindex*sizeof(itemtype));//kopiownie zawartosci
     size+=basesize;       //zarejestrowanie zwiekszonej pojemnosci
     getindex=0;putindex=oldsize;//ustawienie zmiennych sterujacych
     delete []dump;          // zwolnienie starej teblicy
     return 1;             // OK
     }

virtual exfifo& operator << (itemtype tp)
// nowy operator wstawiania - niestety nierozwijalny "inline"
      {
      while(!put(tp))
                  {
                  if(!expand()) error("OUT OF MEMORY AT EXPAND FIFO",1);
                  }
      return *this;
      }
private:
size_t basesize;// Jednostka dlugosci kolejki
};

