/*
Workspace User Interface - test image
*/
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
#include "vesa.hpp"


main()
{
picture *Z=screen::StartMenu();
extern unsigned char far image[32][32];
const mem_picture I((pixpointer)image,31,31);
mem_picture B(34,44);
Z->PutPicture(20,20,I);
getch(); //Stop to keypressed !
//if(ISVESA) //debilizm do Deb.
delete Z;
return 0;
}