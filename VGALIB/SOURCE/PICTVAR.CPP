/*
Workspace User Interface - podstawowe zasoby predefiniowane 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <values.h>
#include <math.h>
#include <mem.h>
#include <conio.h>
#include <dos.h>

#include "biosgraf.h"
//#include "color.hpp"
#include "picture.hpp"
#include "screens.hpp"
#include "fonts.hpp"

palette  DefaultPalette(256,0);

extern char defaultfonttable[];  //Tablica z krojem fontu standartowego
font8bit DefaultFont(defaultfonttable,12); // Podstawowy font wewnetrzny

fontinfo font::FontTypes[16]={{&DefaultFont,1},{NULL,1}};
size_t font::NumberOfTypes=sizeof(FontTypes)/sizeof(FontTypes[0]);
size_t font::CurentNumber=1;

/* Non portable DOS only section */
screeninfo screen::ScreenTypes[]={
      {"Invalid switch 0",NULL},
      {"VGA 320x200 256 colors",ScreenVGA13::StartVGA13},
      {"VESA SVGA   256 colors",ScreenVESA::StartVESA}
      };
size_t screen::NumberOfTypes=sizeof(ScreenTypes)/sizeof(ScreenTypes[0]);

int screen::OldMode=0;
int screen::InGraph=0;
