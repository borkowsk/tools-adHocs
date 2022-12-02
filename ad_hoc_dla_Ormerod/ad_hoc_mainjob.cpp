//Program Ad hoc przetwarzaj¹cy dane z modelu OrmerodsNet
//Autor: Wojciech Borkowski
////////////////////////////////////////////////////////////////////
#include <cassert>
#include <string>
#include <iostream>//Standardowe wejœcie wyjœcie na konsole
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

void Przenies(const TabelaTabDelimited& Zrodlo,unsigned kolumnaZrodla,TabelaTabDelimited& Cel,unsigned kolumnaCelu,unsigned kolumnaMetryczki=-1,unsigned OstatniWierszMetryczki=0)
{
	Cel.PrzekopiujZ(Zrodlo,//Wkleja kolumne od góry
						   0,kolumnaZrodla, //Od
						  -1,kolumnaZrodla, //Do
						  0,kolumnaCelu);//Pozycja docelowa - od samej góry
	if(kolumnaMetryczki!=-1)//Jest metryczka
	{
		Cel.PrzekopiujZ(Zrodlo,//Wkleja metryczkê od góry do OstatniWierszMetryczki
						   0,kolumnaMetryczki, //Od
	  OstatniWierszMetryczki,kolumnaMetryczki, //Do
	   					  0,kolumnaCelu);//Pozycja docelowa - od samej góry
	}
}

int main(int argc, char *argv[], char *envp[])
//NACZELNA FUNKCJA PROGRAMU
{
	unsigned Kolumna=0; //Które kolumny po³¹czyæ?
	unsigned IleKolumn=-1; //-1 oznacza koniecznosc detekcji
	unsigned IleWierszy=-1;//-1 oznacza koniecznosc detekcji
	atexit(zakoncz);

	
    cout<<"/*  Program 'ad hoc' dla danych OrmerodsNets ("<<__DATE__<<")."<<endl;
	cout<<"/*  !!!!    Z korekta bledu dla wersji 2.02b      ."<<endl;
	cout<<"/*==========================================================."<<endl;
	char* buf=NULL;cout<<"/*KATALOG ROBOCZY: "<<(buf=_getcwd(NULL,0))<<"."<<endl; if(buf!=NULL) free(buf);
	//system("dir *.log");

	///////////////////////////////////////////////////////////////////////////////
	if(argc>1) { Kolumna=atol(argv[1]);cerr<<"/* Kolumna:\t"<<Kolumna<<"\t."<<endl;} 
		cerr<<"/* Pliki: "<<endl;

	for(unsigned f=2;f<argc;f++) 
		cerr<<"/*\t"<<argv[f]<<"\tsize:\t"<<file_lenght(argv[f])<<"B\t."<<endl;
    cout<<"NOTE!!! FILES MUST HAVE SAME NUMBER OF COLUMNS!"<<endl<<endl;

	//////////////////////////////////////////////////////////////////////////////
	auto_ptr<TabelaTabDelimited> TabelaWynikowa(new TabelaTabDelimited);//Na pocz¹tku nie wiadomo jaka ma byc du¿a
	TabelaWynikowa->ZmienNazwe("TABELA_WYNIKOWA");

	for(unsigned f=2;f<argc;f++) 
	{
		cout<<endl;
		cout<<"/* Wczytuje tabele danych z "<<argv[f]<<" ."<<endl;
		cout<<"/*------------------------------------------------------"<<" ."<<endl;
		TabelaTabDelimited TabelaAkt;
		TabelaAkt.Opisowo=1;
		if(!TabelaAkt.WczytajZPliku(argv[f],'\t',-1,IleKolumn)) //Kolumn ma byæ tyle samo, ale wierszy nie musi
		{
			cerr<<"Nie warto kontynuowac."<<endl;
			return -1;
		}
		
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//AD HOC KOREKTA B£ÊDU LOGÓW W WERSJI 2.02b
		TabelaAkt(22,0)=TabelaAkt(19,2);
		TabelaAkt(22,1)=TabelaAkt(19,3);

		if(IleKolumn==-1) //Pierwsze podejœcie (tylko pierwszy plik danych)
		{						
			IleKolumn=TabelaAkt.IleKolumn();
			IleWierszy=TabelaAkt.IleWierszy();
			TabelaWynikowa->UstalRozmiar(IleWierszy,argc-1);//+1 bo kolumna 0 z opisem
			cout<<"/* Zaalokowano tabele wynikowa na "<<TabelaWynikowa->IleWierszy()<<" wierszy i "<<TabelaWynikowa->IleKolumn()<<" kolumn"<<endl;
			TabelaWynikowa->PrzekopiujZ(TabelaAkt,0,0,IleWierszy-1,0);//Wkleja kolumne opisu od poczatku
			//cout<<*TabelaWynikowa;
		}
		else //Kolejne pliki danych mog¹ mieæ wiêcej wierszy!!!
			if(TabelaAkt.IleWierszy()>IleWierszy)
			{
				auto_ptr<TabelaTabDelimited> StaraTabelaWynikowa(TabelaWynikowa);//Na pocz¹tku nie wiadomo jaka ma byc du¿a
				StaraTabelaWynikowa->ZmienNazwe("STARA_TABELA_WYNIKOWA");
				auto_ptr<TabelaTabDelimited> tmp(new TabelaTabDelimited);
				TabelaWynikowa=tmp;
				TabelaWynikowa->ZmienNazwe("NOWA_TABELA_WYNIKOWA");
				IleWierszy=TabelaAkt.IleWierszy();
				TabelaWynikowa->UstalRozmiar(IleWierszy,argc-1);//+1 bo kolumna 0 z opisem
				TabelaWynikowa->PrzekopiujZ(*StaraTabelaWynikowa,0,1,-1,-1,0,1);
				TabelaWynikowa->PrzekopiujZ(TabelaAkt,0,0,IleWierszy-1,0);//Wkleja kolumne opisu od poczatku				
			}

	/////////////////////////////////////////////////////
		Przenies(TabelaAkt,Kolumna,*TabelaWynikowa,f-1,1,24);
	/////////////////////////////////////////////////////
	}
	
	cout<<endl;
	{char Bufor[100];
	_itoa_s(Kolumna,Bufor,sizeof(Bufor),10);
	RdzenNazwyTabeliWynikowej+=Bufor;}
	RdzenNazwyTabeliWynikowej+=".dat";
	cout<<"/* Zapisuje tabele wynikowa "<<RdzenNazwyTabeliWynikowej<<" ."<<endl;
	cout<<"/*------------------------------------------------------"<<" ."<<endl;
	TabelaWynikowa->ZapiszDoPliku(RdzenNazwyTabeliWynikowej.c_str());

	return EXIT_SUCCESS;
}

void zakoncz()
//Funkcja robi¹ca planowe porz¹dki
{
     //inne porz¹dki
     //...
	 cerr<<endl<<"Czekam na 'klik'!"<<endl;
	 cin.get();//U¿ytkownik ma szanse przeczytaæ
}      


/////////////////////////////////////////////////////////////////
//  Wojciech Borkowski
//  Instytut Studiów Spo³ecznych Uniwersytet Warszawski
//  http://borkowski.iss.uw.edu.pl
/////////////////////////////////////////////////////////////////