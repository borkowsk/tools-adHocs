//Program Ad hoc przetwarzaj¹cy WERTYKALNIE dane z modelu OrmerodsNet 
//Autor: Wojciech Borkowski
//////////////////////////////////////////////////////////////////////
#include <cassert>
#include <string>
#include <iostream>//Standardowe wejœcie wyjœcie na konsole
#include <fstream>
#include <direct.h> //getcwd()
#include <sys/types.h>
#include <sys/stat.h>
#include <memory>
#include "TabelaTabDeli.h"

using namespace std; //£atwe u¿ycie nazw z przestrzeni "std"

string RdzenNazwyTabeliWynikowej="OrmerodNetsResults";

off_t file_lenght(const char* filename)
{
	struct _stat buf;
    int result;
	result = _stat( filename, &buf );
	if( result != 0 ) 
	{
		cerr<<endl<<"!!!Problem getting information! ";
		perror(filename);
		return 0;
	}
	else
		return buf.st_size;
}

void zakoncz(); //Funkcja robi¹ca planowe porz¹dki i czekaj¹ca na "klik"

void Naglowek(const TabelaTabDelimited& Zrodlo,TabelaTabDelimited& Cel)
{
	Cel(0,0)=Zrodlo(25,24);//+TabelaAkt(26,1);
	Cel(0,1)=Zrodlo(2,0);//Topology
	Cel(0,2)="Self Up";//Zrodlo(,);//
	Cel(0,3)="Self Down";
	Cel(0,4)="SELF CODE";
	Cel(0,5)="N.of SEEDS";
	Cel.PrzekopiujZ(Zrodlo,25,14,25,23,0,6);
	Cel.PrzekopiujZ(Zrodlo,25,1,25,12,0,16);
}

string ZrobKodSelfu(const string& cUp,const string& cDown)
{
	string Up,Down;
	if(cUp=="0")
	{
		Up=cUp+".00";
	}
	else
	Up=cUp;

	if(cDown=="0")
	{
		Down=cDown+".00";
	}
	else
	Down=cDown;
	
	string Ret; //UWAGA PARTYZANTKA - ZADZIA£A TYLKO DLA ODPOWIEDNICH DANYCH
	Ret="     ";//Zamiast allokacji :-)
	Ret[0]=Up[2];Ret[1]=(Up[3]=='\0'?'0':Up[3]);
	Ret[2]=Down[2];
	Ret[3]=(Down[3]=='\0'?'0':Down[3]);
	Ret[4]='\0';
	return Ret;
}

void Przenies(const TabelaTabDelimited& Zrodlo,TabelaTabDelimited& Cel,unsigned Wstart=0)
{
	unsigned Wlast=Zrodlo.IleWierszy()-27-2;//27 wierszy ma metryczka na g³upi¹ koncówkê
	Cel(Wstart,0)=Zrodlo(26,24)+string("-")+Zrodlo(26,0);
	Cel(Wstart,1)=Zrodlo(2,1);//Topology
	Cel(Wstart,2)=Zrodlo(19,1);//"Self Up"
	Cel(Wstart,3)=Zrodlo(22,1);//"Self Down";
	Cel(Wstart,4)=ZrobKodSelfu(Cel(Wstart,2),Cel(Wstart,3));//"SELF CODE";
	Cel(Wstart,5)=Zrodlo(26,8);//"N.of SEEDS";
	Cel.PrzekopiujZ(Zrodlo,26,14,26,23,Wstart,6);
	Cel.PrzekopiujZ(Zrodlo,26,1,26,12,Wstart,16);
	
	for(unsigned W=1;W<=Wlast;W++)//Namno¿enie nag³ówka
	{
		Cel(Wstart+W,0)=Zrodlo(26+W,24)+string("-")+Zrodlo(26+W,0);
		Cel(Wstart+W,1)=Zrodlo(2,1);//Topology
		Cel(Wstart+W,2)=Zrodlo(19,1);//"Self Up"
		Cel(Wstart+W,3)=Zrodlo(22,1);//"Self Down";
		Cel(Wstart+W,4)=Cel(Wstart,4);//ZrobKodSelfu(Cel(Wstart,2),Cel(Wstart,3));//"SELF CODE";
		Cel(Wstart+W,5)=Zrodlo(26+W,8);//"N.of SEEDS";
	}
	Cel.PrzekopiujZ(Zrodlo,27, 8,26+Wlast,	    Wstart+1, 5);
	Cel.PrzekopiujZ(Zrodlo,27,14,26+Wlast,23,	Wstart+1, 6);
	Cel.PrzekopiujZ(Zrodlo,27, 1,26+Wlast,12,	Wstart+1,16);
}

int main(int argc, char *argv[], char *envp[])
//NACZELNA FUNKCJA PROGRAMU
{
	unsigned Kolumna=0; //Które kolumny po³¹czyæ?
	unsigned IleKolumn=-1; //-1 oznacza koniecznosc detekcji
	unsigned IleWierszy=-1;//-1 oznacza koniecznosc detekcji
	atexit(zakoncz);

	
    cerr<<"* Program 'ad hoc B' dla danych OrmerodsNets ("<<__DATE__<<")."<<endl;
	cerr<<"*Komasowanie plikow log w jeden strawny dla SPS'a"<<endl;
	cerr<<"*=====================================================================."<<endl;
	char* buf=NULL;cerr<<"*KATALOG ROBOCZY: "<<(buf=_getcwd(NULL,0))<<"."<<endl; if(buf!=NULL) free(buf);
	//system("dir *.log");

	///////////////////////////////////////////////////////////////////////////////
	cerr<<"* Pliki: "<<endl;
	for(unsigned f=1;f<argc;f++) 
		cerr<<"*\t"<<argv[f]<<"\tsize:\t"<<file_lenght(argv[f])<<"B\t."<<endl;
    cerr<<"**NOTE!!! FILES MUST HAVE SAME NUMBER OF COLUMNS!"<<endl;
//	cerr<<"          RESULTS GO TO STD.OUTPUT. USE > FOR REDIRECT"<<endl<<endl;

	////////////////////////////
    //Otwoarcie pliku na wyniki
	//////////////////////////////////////////////////////////////////////////////
	ofstream Out("OrmerodForSPSS.dat");
	if(!Out.is_open())
	{
		perror("OrmerodForSPSS.dat");
		cerr<<"!!! Nie udalo sie. Nie warto kontynuowac."<<endl;
		return EXIT_FAILURE;
	}
	auto_ptr<TabelaTabDelimited> TabelaWynikowa(new TabelaTabDelimited);//Na pocz¹tku nie wiadomo jaka ma byc du¿a
	TabelaWynikowa->ZmienNazwe("TABELA_WYNIKOWA");

	for(unsigned f=1;f<argc;f++) 
	{
		cerr<<endl;
		cerr<<"* Wczytuje tabele danych z "<<argv[f]<<" ."<<endl;
		cerr<<"*------------------------------------------------------"<<" ."<<endl;
		TabelaTabDelimited TabelaAkt;
		TabelaAkt.Opisowo=1;
		if(!TabelaAkt.WczytajZPliku(argv[f],'\t',-1,IleKolumn)) //Kolumn ma byæ tyle samo, ale wierszy nie musi
		{
			cerr<<"!!! Nie udalo sie. Nie warto kontynuowac."<<endl;
			return EXIT_FAILURE;
		}
		
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//AD HOC KOREKTA B£ÊDU LOGÓW W WERSJI 2.02b
		TabelaAkt(22,0)=TabelaAkt(19,2);
		TabelaAkt(22,1)=TabelaAkt(19,3);
			
		IleWierszy=TabelaAkt.IleWierszy();
		IleKolumn=TabelaAkt.IleKolumn();	

		if(f==1) //Pierwsze podejœcie (tylko pierwszy plik danych ma byæ z nag³ówkiem)
		{
			TabelaWynikowa->UstalRozmiar(IleWierszy-24-3,IleKolumn+5);//WYBRANE KOLUMNY - WIERSZY MNIEJ O METRYCZKE
			cerr<<"* Zaalokowano tabele wynikowa na "<<TabelaWynikowa->IleWierszy()
				<<" wierszy i "<<TabelaWynikowa->IleKolumn()<<" kolumn"<<endl;
			Naglowek(TabelaAkt,*TabelaWynikowa);
			Przenies(TabelaAkt,*TabelaWynikowa,1);//Dane od pierwszego wiersza
		}
		else
		{	
			TabelaWynikowa->UstalRozmiar(IleWierszy-25-3,IleKolumn+5);//WYBRANE KOLUMNY - WIERSZY MNIEJ O METRYCZKE I NAG£ÓWEK
			cerr<<"* Zaalokowano tabele wynikowa na "<<TabelaWynikowa->IleWierszy()
				<<" wierszy i "<<TabelaWynikowa->IleKolumn()<<" kolumn"<<endl;
			//Bez nag³ówka ale ...
			Przenies(TabelaAkt,*TabelaWynikowa,0);//... dane od pocz¹tkowego wiersza
		}

		Out<<*TabelaWynikowa; //TYLKO TU U¯YTY PLIK OUT !!!
	}
	
	return EXIT_SUCCESS;
}

void zakoncz()
//Funkcja robi¹ca planowe porz¹dki
{
     //inne porz¹dki
     //...
	 cerr<<endl<<"* Czekam na 'klik'!"<<endl;
	 cin.get();//U¿ytkownik ma szanse przeczytaæ
}      


/////////////////////////////////////////////////////////////////
//  Wojciech Borkowski
//  Instytut Studiów Spo³ecznych Uniwersytet Warszawski
//  http://borkowski.iss.uw.edu.pl
/////////////////////////////////////////////////////////////////