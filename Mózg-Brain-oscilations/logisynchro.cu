//     Implementacja wielu r�wnolegle dzia�aj�cych uk�ad�w logistycznych sprz�onych s�siadami
//						na multiprocesory NVIDIA w j�zyku CUDA C
//      Wojciech Borkowski - Instytut Studi�w Spo�ecznych Uniwersytet Warszawski
//					       www.iss.uw.edu.pl/borkowski
///////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>
#include "cutil_inline.h"
#include "symshell.h" //Naglowek "prymityw�w graficznych" biblioteki SYMSHELL

struct Image {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[496 * 496 * 3];
}; 
extern "C" const Image gimp_image;

const int  LAMBDA_SELECT=1;      //Czy Lambdy losowe(0) czy inne(1) czy z obrazka(2)
const bool USE_TRUE_COLOR=true; //Czy w trybie RGB czy paletowo?
const bool SAVE_TO_FILES=false;	 //Czy zapisywać ekrany do plików?

typedef float REAL;//Nie dzia�a dla innego typu na starszych kartach (czyli wi�kszo�ci z TESL� w��cznie)
#define ALFA (REAL(3.0/10.0))//Ile bierze swojego x z poprzedniegop kroku. Od s�siad�w 1-ALFA
					//Powinno by� const a nie #define, ale wtedy nie widac w kodzie CUDA (blad kompilatora?)

size_t SIDELENGTH=31*16;//Dlugosc boku kwadratowej tablicy kom�rek - lepiej jak wielokrotno�� 16
size_t WORLDSIZE=SIDELENGTH*SIDELENGTH;//Ile jest kom�rek w sumie w kwadratowej tablicy - bo alkoacja tu tylko liniowa
size_t MAXINDEX=SIDELENGTH-1;	//Maksymalny dozwolony index

dim3 ThreadsPerBlock(4, 4); //Ile w�tk�w w ka�dym z blok�w 
dim3 Grid(SIDELENGTH / ThreadsPerBlock.x, SIDELENGTH / ThreadsPerBlock.y);//Na ile blok�w trzeba podzieli� macie�
//int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;//Jak niepodzielne przez 16
  
int MAXSTEPS=100000;//Ile zrobi� krok�w
int world_count=0;//Ile ju� zrobiono krok�w
clock_t timer=0;//Czas wykonania kernela z punktu widzenia systemu
int vis_freq=2;//max(1,min(MAXSTEPS-1,1));//Co ile krok�w wizualizacja - nie mniej ni� 1
int VSIZ=2;//Szeroko�� kwadracika - na razie nie u�ywane

//extern "C" ???
int WB_error_enter_before_clean=1;

// "Swiat" jest co prawda kwadratowy, ale jest alokowany jako liniowy blok, bo tak tylko umie CUDA
// wiec zeby myslec o nim jako o miaciezy kwadratowej trzeba recznie przeliczac funkcja getval(row,col)
//////////////////////////////////////////////////////////////////////////////////////////////////////////
REAL* worldA=NULL;//Stan aktualny/nastepny
REAL* worldB=NULL;//Stan aktualny/nastepny drugi (u�ywane na zmiane)
REAL* Lambdy=NULL;//Wartosci parametru kontroli
unsigned* Far=NULL;//Dalekie linki

__host__ __device__ inline
int Ind(int row,int col,size_t SIDELENGTH)
{
	return row*SIDELENGTH+col;
}

__host__ inline //Wersja dla glownego procesora
REAL& getval(REAL* world,int row,int col)//Zwraca referencje, wi�c sluzy do czytania i pisania
{/*
							assert(row>=0);
							assert(row<SIDELENGTH);
							assert(col>=0);
							assert(col<SIDELENGTH); */
	int index=row*SIDELENGTH+col;
	return world[index];
}

__device__ //Wersja dla CUDA
REAL& _getval(REAL* world,int row,int col,size_t SIDELENGTH)//Zwraca referencje, wi�c sluzy do czytani i pisania
{						
	int index=row*SIDELENGTH+col;
	return world[index];
}

//Funkcje dla glownego procesora do zainicjowania tablic "swiata"
///////////////////////////////////////////////////////////////////////
void setvalues(REAL* world,REAL val) //Ta sama wartosc
{
						assert(world!=NULL);
	for(size_t i=0;i<WORLDSIZE;i++)
		world[i]=val;
}

void randvalues(REAL* world) //Wartsci losowe
{
						assert(world!=NULL);
	for(size_t i=0;i<WORLDSIZE;i++)
		world[i]=double(rand())/double(RAND_MAX);
}

void initvaluesvert(REAL* world,double maxval=1) //Gradient wertykalny
{
						assert(world!=NULL);
	for(size_t j = 0;j<SIDELENGTH;j++)//Kolumny-komorki w wierszu
	{
		double pom=double(j)/double(SIDELENGTH)*maxval;
		for(size_t i = 0;i<SIDELENGTH;i++)//Kolejne wiersze
		{
			getval(world,i,j)=pom;
		}
	}
}

void initvalueshoriz(REAL* world,double maxval=1) //Gradient horyzontalny
{
							assert(world!=NULL);
	for(size_t i = 0;i<SIDELENGTH;i++)//Kolejne wiersze
	{
		double pom=double(i)/double(SIDELENGTH)*maxval;
		for(size_t j = 0;j<SIDELENGTH;j++)//Kolumny-komorki w wierszu
		{
			getval(world,i,j)=pom;
		}
	}
}

void initvaluesfromImage(REAL* table,const Image& the_image)
{
										assert(the_image.width==SIDELENGTH);
										assert(the_image.height==SIDELENGTH);
	for(size_t i = 0;i<SIDELENGTH;i++)//Kolejne wiersze
	{
		for(size_t j = 0;j<SIDELENGTH;j++)//Kolumny-komorki w wierszu
		{
			unsigned first=(i*SIDELENGTH+j)*the_image.bytes_per_pixel;
			double pom=the_image.pixel_data[first]+the_image.pixel_data[first+1]+the_image.pixel_data[first+2];
			pom/=3;//srednia jasnosc
			pom/=255; //Zakres 0..1
			getval(table,i,j)=pom;
		}
	}
}

void initfarconnections(unsigned* Far,REAL* Lambdy)
{
								assert(Lambdy!=NULL);
								assert(Far!=NULL);
	unsigned a,b;
	for(size_t i = 0;i<SIDELENGTH;i++)//Kolejne wiersze
	{
		for(size_t j = 0;j<SIDELENGTH;j++)//Kolumny-komorki w wierszu
		if(getval(Lambdy,i,j)!=0)
		{
			do {
				double angle=double(rand())/double(RAND_MAX)*M_PI*2;
				double r=(double(rand())/double(RAND_MAX))*
						 (double(rand())/double(RAND_MAX))*
						 (double(rand())/double(RAND_MAX))*
						 (double(rand())/double(RAND_MAX))*
						 (double(rand())/double(RAND_MAX))*
						 (double(rand())/double(RAND_MAX))*
						 SIDELENGTH/2;
				a=(SIDELENGTH+i+r*sin(angle));
					a%=SIDELENGTH;
				b=(SIDELENGTH+j+r*cos(angle));
					b%=SIDELENGTH;
			}while(getval(Lambdy,a,b)==0);

			line(j,i,b,a,1+(i*j)%254);
			Far[Ind(i,j,SIDELENGTH)]=a*SIDELENGTH+b;
		}
		else Far[Ind(i,j,SIDELENGTH)]=0;

		flush_plot();
	}
}

//DO SYMULACJI NA CUDA
////////////////////////////
__global__ //Definicja tego co robi jeden watek - obliczaj�cy nowy stan jednej kom�rki
void kernel_mem(REAL* old_world, REAL* world, REAL* Lambdy, unsigned* Far, size_t SIDELENGTH, unsigned int stepcount)
{   
	unsigned int i = blockIdx.x*blockDim.x + threadIdx.x;//Mapowanie 'i' ze stand. struktur opisu gridu watk�w
	unsigned int j = blockIdx.y*blockDim.y + threadIdx.y;//Mapowanie 'j' ze stand. struktur
	unsigned int index=Ind(i,j,SIDELENGTH);

	REAL x=old_world[index];//_getval(old_world,i,j,SIDELENGTH);
	REAL r=Lambdy[index];//_getval(Lambdy,i,j,SIDELENGTH);
	if(r<0.1) r=0;
		else	r=3.5+r/2;//sqrt(r*4);
	REAL xs=0;//Na "magazynowanie" wp�ywu s�siad�w von Neumana r=2 (krzyzyk)
			  //Nieistniejacy sasiedzi maja zawsze stan = 0
	//Bliscy sasiedzi
	unsigned int LIM=SIDELENGTH-1;
	if(i<LIM) xs+=_getval(old_world,i+1,j,SIDELENGTH);
	if(j<LIM) xs+=_getval(old_world,i,j+1,SIDELENGTH);
	if(i>0) xs+=_getval(old_world,i-1,j,SIDELENGTH);
	if(j>0) xs+=_getval(old_world,i,j-1,SIDELENGTH);
	//Troch� dalsi s�siedzi
	LIM=SIDELENGTH-2;
	if(i>1) xs+=_getval(old_world,i-2,j,SIDELENGTH);
	if(j>1) xs+=_getval(old_world,i,j-2,SIDELENGTH);
	if(i<LIM) xs+=_getval(old_world,i+2,j,SIDELENGTH);
	if(j<LIM) xs+=_getval(old_world,i,j+2,SIDELENGTH);
    //Najdalszy "s�siad" - dalekie po��czenie
	LIM=Far[index];
	
	if(LIM>0)
	{
		xs+=old_world[LIM]*2;
		xs/=10;//Sredni wp�yw od s�siad�w
	} else xs/=8;

	x=ALFA*x+(1.0-ALFA)*xs;//Realizacja wp�ywu s�siad�w
	world[index]=(1-x)*x*r;
}

//DO SYMULACJI NA G��WNYM PROCESORZE - W CELU POR�WNANIA
//////////////////////////////////////////////////////////////
__host__
void pseudo_kernel(REAL* old_world, REAL* world, REAL* Lambdy, size_t SIDELENGTH, unsigned int stepcount)
{
	for(unsigned int j = 0;j<SIDELENGTH;j++)//Kolumny-komorki w wierszu
		for(unsigned int i = 0;i<SIDELENGTH;i++)//Kolejne wiersze
			//Wszystko jedno co pierwsze - gdy ca�a tablica w cashu, ale jak nie?
		{
			REAL x=getval(old_world,i,j);
			REAL r=getval(Lambdy,i,j);
			if(r<0.1) r=0;
			else	r=3+r;//sqrt(r*4);
			REAL xs=0;//Na "magazynowanie" wp�ywu s�siad�w von Neumana r=2 (krzyzyk)
					  //Nieistniejacy sasiedzi maja zawsze stan = 0
			unsigned int LIM=SIDELENGTH-1;
			if(i<LIM) xs+=getval(old_world,i+1,j);
			if(j<LIM) xs+=getval(old_world,i,j+1);
			if(i>0) xs+=getval(old_world,i-1,j);
			if(j>0) xs+=getval(old_world,i,j-1);
			LIM=SIDELENGTH-2;
			if(i>1) xs+=getval(old_world,i-2,j);
			if(j>1) xs+=getval(old_world,i,j-2);
			if(i<LIM) xs+=getval(old_world,i+2,j);
			if(j<LIM) xs+=getval(old_world,i,j+2);
			
			xs/=8;//Sredni wp�yw od s�siad�w
			x=ALFA*x+(1.0-ALFA)*xs;//Realizacja wp�ywu s�siad�w
			getval(world,i,j)=(1-x)*x*r;
		}
}

// FUNKCJE OPAKOWANIA - DO WIZUALIZACJI I STEROWANIA
///////////////////////////////////////////////////////////////////

//Drukowanie malych tablic tekstowo
void print_world(REAL* world)
{
						assert(world!=NULL);
	printf("\n\f%6d\n",world_count);
	for(unsigned  i=0;i<SIDELENGTH;i++)//KOlejne wiersze
	{
		for(unsigned j=0;j<SIDELENGTH;j++)//Kolumny-komorki w wierszu
		{
			//printf("%2d",getval(world,i,j));
			int c=32+getval(world,i,j)*64;
			printf("%c",c);
		}
		printf("!\n");
	}
}

// Wyswietlanie duzych tablic z uzyciem grafiki symshell'a
void display(REAL* world,double maxval=1,bool Yellow=0)
{
											assert(world!=NULL);
	int end=screen_width();
	if(end>SIDELENGTH)
			end=SIDELENGTH;
	printc(0,end,128,255,"%6d",world_count);
	if(Yellow)
	{
		for(int i=0;i<end;i++)//KOlejne wiersze
		{
			for(int j=0;j<end;j++)//Kolumny-komorki w wierszu
			{
				double pom=getval(world,i,j);
				pom/=maxval;
				pom*=255;
				if(!USE_TRUE_COLOR)
					plot(j,i,pom); //Wersja z domyslna paleta kolorow
				else
					plot_rgb(j,i,pom,pom,0);//Wersja w "zlotych szarosciach" RGB
			}
		}
	}
	else
	{
		for(int i=0;i<end;i++)//KOlejne wiersze
		{
			for(int j=0;j<end;j++)//Kolumny-komorki w wierszu
			{
				double pom=getval(world,i,j);
				pom/=maxval;
				pom*=255;
				if(!USE_TRUE_COLOR)
					plot(j,i,pom);//Wersja z domyslna paleta kolorow
				else
					plot_rgb(j,i,pom,0,pom);//Wersja we "fioletowych szarosciach" RGB
			}
		}
	}
	printc(0,end-char_height('1'),240,0,"%6d",world_count);
	const char* podpis="wborkowski@swps.edu.pl";
	printc(SIDELENGTH-string_width(podpis),end-string_height(podpis),256+28,0,"%s",podpis);
	flush_plot();//Obrazek juz gotowy - wyswietlic!
}


//PRINT or/and DISPLAY HELP
void Help()
{
	//...
}


//Sprawdzanie wejscia z okna graficznego - dosyc podstawowe
int check_gr_input()
{
	char tab[2];
	tab[1]=0;
	if(input_ready())//Jesli przyszedl jakis komunikat
	{
		tab[0]=get_char();
		switch(tab[0])
		{
			case '@'://Wymuszony replot ekranu - tu drukowanie Lambd
			case '\r':display(Lambdy,1,true);break;
			case 'q'://Przerwanie aktualnej funkcji symulacji lub wyjscie z programu
			case EOF:return -1;
			case 'h'://Drukowanie helpu jak przyszedl niezdefiniowany w switch'u komunikat
			default:
					Help();break;
		}
	}
	//DOMYSLNE
	return 0; //Jak nie przyszedl lub zostal poprawnie obsluzony
}


//"PROCEDURY" STEROWANIA WYKONANIEM SYMULACJI NA CUDA'IE i NA GLOWNYM CPU
////////////////////////////////////////////////////////////////////////////

void run_on_device() //Wykonanie na CUDA
{
								assert(worldA!=NULL);  //Swiat w glownej pamieci musi byc juz gotowy 
								assert(worldB!=NULL);	
								assert(Lambdy!=NULL);
								assert(Far!=NULL);
	//Wskazniki do wersji/kopii swiata w pamieci karty [obliczeniowej]
	REAL* worldAdev=NULL;
	REAL* worldBdev=NULL;
	REAL* LambdyDev=NULL;
	unsigned* FarDev=NULL;

	// Allocate vectors in device memory
    cutilSafeCall( cudaMalloc((void**)&worldAdev, WORLDSIZE*sizeof(worldAdev[0]) ) );
    cutilSafeCall( cudaMalloc((void**)&worldBdev, WORLDSIZE*sizeof(worldBdev[0]) ) );
	cutilSafeCall( cudaMalloc((void**)&LambdyDev, WORLDSIZE*sizeof(LambdyDev[0]) ) );
	cutilSafeCall( cudaMalloc((void**)&FarDev, WORLDSIZE*sizeof(FarDev[0]) ) );
	// Copy vector A from host memory to device memory
    cutilSafeCall( cudaMemcpy(worldAdev, worldA, WORLDSIZE*sizeof(worldAdev[0]), cudaMemcpyHostToDevice) );	
	// Copy table of parameters (Lambdy) into device memory
	cutilSafeCall( cudaMemcpy(LambdyDev, Lambdy, WORLDSIZE*sizeof(LambdyDev[0]), cudaMemcpyHostToDevice) );
	// Copy table of far connections
	cutilSafeCall( cudaMemcpy(FarDev, Far, WORLDSIZE*sizeof(FarDev[0]), cudaMemcpyHostToDevice) );

	//DEBUG ONLY - z powrotem do g�ownej pamieci dla sprawdzenia
	//cutilSafeCall( cudaMemcpy(worldA,worldAdev, WORLDSIZE*sizeof(worldAdev[0]), cudaMemcpyDeviceToHost) );
	
	if(SIDELENGTH<64) //Co� na EKRANIK TEKSTOWY - jesli si� zmiesci
		print_world(worldA);

	display(worldA); //Stan poczatkowy na ekranie graficznym

	//Poczatkowe przypisanie "swiatow" w pamieci karty
	REAL* old_world=worldAdev;
	REAL* new_world=worldBdev;
	
	timer=clock(); //Poczatek liczenia sumarycznego czasu wykonania wzgledem procesora glownego - dziala tylko zgrubnie 

	//PETLA SYMULACJI NA CUDA:
    while(world_count < MAXSTEPS) 
    {
		// Run one step calculation on device - Grid & ThreadsPerBlock defined much obove
        kernel_mem<<< Grid,ThreadsPerBlock >>>(old_world, new_world, LambdyDev, FarDev, SIDELENGTH,world_count+1);
        cutilCheckMsg("kernel_mem launch failure");

		//Kernel jest wzywany asynchronicznie, wi�c "za darmo" mo�na co� tymczasem sprawdzi� i nawet mo�e odrysowa�
		if(check_gr_input()==-1) goto KONIEC_PIESNI;//Moze "skoczyc do wyjscia"
		
#ifdef DEBUG
		cutilSafeCall( cudaThreadSynchronize() );//Zeby dzialalo poprawnie debugowanie to trzeba poczekac az skonczy
#endif

		++world_count;//Zliczanie krok�w symulacji
		
		if(world_count%vis_freq==0) //Sci�gniecie danych czasochlonne - wiec tylko dla niektorych krokow jest wizualizacja
		{	
			// Copy result from device memory to host memory for visualisation (SYNCHRONISATION BY DEFAULT)
			cutilSafeCall( cudaMemcpy(worldB,new_world, WORLDSIZE*sizeof(worldB[0]), cudaMemcpyDeviceToHost) );
			if((SIDELENGTH<64)) print_world(worldB);//Cos na konsole tekstowa
				//else printf("\r%4d",world_count);
			display(worldB);//Wyswietlenie danych z poprzedzajacego czytania
			if(SAVE_TO_FILES)
			{
				char Filename[256];
				sprintf(Filename,"LoSyn%08d",world_count);
				dump_screen(Filename);
			}
		}
		
		//Preparing for next step - swaping vectors
		REAL* pom=old_world;
		old_world=new_world;
		new_world=pom;
    }

KONIEC_PIESNI:
	printf("\n%g ms/step",(clock()-timer)/double(world_count));//Podliczenie sumarycznego czasu wykonania

    // Niezbedne porzadki na karcie (free device memory)
    if (worldAdev)
        cudaFree(worldAdev);
    if (worldBdev)
        cudaFree(worldBdev);
	if(LambdyDev)
		cudaFree(LambdyDev);
	if(FarDev)
		cudaFree(FarDev);
}

void run_on_host( ) //Dla porownania wykonanie na procesorze glownym
{
								assert(worldA!=NULL);
								assert(worldB!=NULL);		
	if((SIDELENGTH<64)) print_world(worldA);//Co� na EKRANIK TEKSTOWY
	display(worldA); //Stan poczatkowy na ekranie graficznym

	REAL* old_world=worldA;//Poczatkowe przypisanie "swiatow"
	REAL* new_world=worldB;//... w pamieci gl�wnej (CPU)
	
	timer=clock();//Poczatek liczenia sumarycznego czasu wykonania wzgledem procesora glownego

	//PETLA SYMULACJI NA CPU:
    while(world_count < MAXSTEPS) 
    {
		// Run calculation on main CPU
        pseudo_kernel(old_world, new_world, Lambdy, SIDELENGTH,world_count+1);

		if(check_gr_input()==-1) goto KONIEC_PIESNI;//Moze "skoczyc do wyjscia"

        ++world_count;//Zliczanie krok�w symulacji
		
		if(world_count%vis_freq==0)//Wizualizacja tez tylko od czasu do czasu
		{
			if((SIDELENGTH<64)) print_world(new_world);
			else printf("\r%4d",world_count);
			display(new_world);
		}
		
		//Preparing for next step - swaping vectors and increment counters
		REAL* pom=old_world;
		old_world=new_world;
		new_world=pom;
    }
KONIEC_PIESNI:
	printf("\n%g ms/step",(clock()-timer)/double(world_count));//Podliczenie czasu i juz
}

// Main host code
///////////////////////////////////////
int main(int argc, const char** argv)
{
	//Gdyby chcie� zaczyna� z warto�ci losowych
	srand(time(NULL));//No chyba, �e by si� chcia�o powtarzalnie debugowa� wtedy jaka� liczba zamias time(NULL)
	//srand(10);

	mouse_activity(0);   /* Mysz w obrebie okna nie bedzie potrzebna */
	set_background(255); /* Biale tlo */
	buffering_setup(1);  /* Wlaczone buforowanie do animacji - widac dopiero jak obrazek gotowy */
	shell_setup("Logistic iterations - synchronisation",argc,argv); /* Tytul okna i przekazanie parametr�w wywolania */

    if(!init_plot(SIDELENGTH,SIDELENGTH,0,0)) //Otwarcie okna z miejscem na g��wn� wizualizacj� swiata i jakie� boki
	{
		fprintf(stderr,"Can't initialize graphics\n");//Jak sie nie udalo...		
		exit(-11);
	}

	printf("\nSpace of coupled logistic iterators %dx%d (%d = %d bajts)\n",SIDELENGTH,SIDELENGTH,WORLDSIZE,WORLDSIZE*sizeof(REAL));
	//Alokacja tablic "Swiat" w glownej pamieci
	worldA=new REAL[WORLDSIZE];
	worldB=new REAL[WORLDSIZE];
	Lambdy=new REAL[WORLDSIZE];
	Far=new unsigned[WORLDSIZE];
	
	//Test symulacji na CUDA
	//////////////////////////////
	printf("\nCalculation on CUDA compatible card (REAL):\n");
	world_count=0;//Ile ju� zrobiono krok�w
	//initvalueshoriz(worldA);//ALTERNATYWNIE
	randvalues(worldA);//ALTERNATYWNIE
	switch(LAMBDA_SELECT){
	default:
	case 0: randvalues(Lambdy);break;//ALTERNATYWNIE
	case 1: initvaluesvert(Lambdy);break;//ALTERNATYWNIE
	case 2: initvaluesfromImage(Lambdy,gimp_image);break;
    }
	initfarconnections(Far,Lambdy);
	setvalues(worldB,0);//Zerowanie tablicy na pierwsze wyniki
	run_on_device(); //WLASCIWA SYMULACJA
/*
	//Test symulacji na jednym rdzeniu CPU
	////////////////////////////////////////////////////
	printf("\nCalculation on one core of main CPU:\n");
	world_count=0;//Ile ju� zrobiono krok�w
	//initvalueshoriz(worldA);
	initvaluesvert(Lambdy);
	setvalues(worldB,0);
	run_on_host();
*/
	//Porzadki na zakonczenie calego programu
	delete worldA;//zwalnianie pamieci
	delete worldB;//zwalnianie pamieci
	delete Lambdy;//zwalnianie pamieci
	delete Far;//zwalnianie pamieci
	
	close_plot(); //Zamkniecie okna graficznego

	return 0;
}

/* 
__global__ //Kernel testowy do sprawdzania czy bloki i watki sie poprawnie mapuja
void kernel_test(REAL* old_world, REAL* world, REAL* Lambdy, size_t SIDELENGTH, unsigned int stepcount)
{
	unsigned int i = blockIdx.x*blockDim.x + threadIdx.x;
	unsigned int j = blockIdx.y*blockDim.y + threadIdx.y;
	
	int index=j*SIDELENGTH+i;
	//world[index]=index;
	
	if(stepcount==1)
		world[index]=threadIdx.x;//TEST 1
	else
	if(stepcount==2)
		world[index]=threadIdx.y;//TEST 1B
	else
	if(stepcount==3)
		world[index]=blockIdx.x;//TEST 2
	else
	if(stepcount==4)
		world[index]=blockIdx.y;//TEST 2B
	else
	if(stepcount==5)
		world[index]=i;//TEST 3
	else
	if(stepcount==6)
		world[index]=j;//TEST 3B
	else
		world[index]=blockDim.x;//TEST 4
	//world[index]=blockIdx.y;//TEST 4B
	//
}
*/
