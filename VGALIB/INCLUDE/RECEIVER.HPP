/* 
Biblioteka  Workspace User Interface
Klasa posredniczaca we wprowadzaniu informacji
z myszy i klawiatury
*/
#include "hardtypes.h"
class receiver
{
void SetEchoWindow(picture& N);//Ustawianie okna odbierajacego echo
virtual int getch();//pobieranie znakow bez echa
virtual int getche();//pobieranie znakow z echem
int MouseInit();
int AboutMouse(fullpixco_t& x,fulpixco_t& y,unsigned ps);
};