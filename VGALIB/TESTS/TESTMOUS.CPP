/*
 Mouse test WUI - procedury obslugi myszy w trybie graficznym
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
#include "mouse.hpp"

#define BREAK if(kbhit()){getch();break;}

main()
{
picture* Z=screen::StartMenu();
MouseDriver Mouse;
picture &S=*Z;
Window W(S,10,10,S.GetMaxX()/2,S.GetMaxY()/2);
Mouse.Init(0);
Mouse.ShowCursor();
S<<"Move your mouse";
delay(5000);
Mouse.HideCursor();
S<<endl<<"Cursor is invisible";
delay(1500);
Mouse.ShowCursor();
S<<endl<<"and cursor is visible";
delay(2000);
return 0;
}