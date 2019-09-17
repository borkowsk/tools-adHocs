/*
Biblioteka Workspace User Interface - klasy obslugiwanych ekranow
*/
#include <dos.h>

class ScreenVGA13:  public mem_picture, public screen
{
ScreenVGA13();
~ScreenVGA13() /* Destruktor z uwalnianiem ekranu */
	{
	BiosScreen( FreeScreen() ); /* Odblokowanie ekranu i przywrocenie statrego trybu */
	}
public:
static picture* StartVGA13() /* Dynamiczna alokacja deskryptora ekranu */
	{
	if(InGraph==0)
		return new ScreenVGA13;
		else
		return NULL;
	}
};


class ScreenVESA:  public picture, public screen
{
ScreenVESA();
void PlotPixel(pixsize_t x,pixsize_t y);
pixpointer CalcPixelAddr(pixsize_t& x,pixsize_t& y) const;
//int GetPicture(const picture& ,fullpixco_t ,fullpixco_t );  // Copy from another picture
void ToTarget()
	{	}
~ScreenVESA() /* Destruktor z uwalnianiem ekranu */
	{
	BiosScreen( FreeScreen() ); /* Odblokowanie ekranu i przywrocenie statrego trybu */
	}
public:
static picture* StartVESA(); /* Dynamiczna alokacja deskryptora ekranu */
};


class ScreenParadise:  public picture, public screen
{
ScreenParadise();
void PlotPixel(pixsize_t x,pixsize_t y);
pixpointer CalcPixelAddr(pixsize_t& x,pixsize_t& y) const;
//int GetPicture(const picture& ,fullpixco_t ,fullpixco_t );  // Copy from another picture
void ToTarget()
	{	}
~ScreenParadise() /* Destruktor z uwalnianiem ekranu */
	{
	BiosScreen( FreeScreen() ); /* Odblokowanie ekranu i przywrocenie statrego trybu */
	}
public:
static picture* StartParadise(); /* Dynamiczna alokacja deskryptora ekranu */
};

