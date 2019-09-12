#include <dos.h>
#include <math.h>
#include <conio.h>

int 	screen(int& line, int column)
{
   char buff[1];

   gettext(column,line,column,line,buff);
   return buff[0];
}
