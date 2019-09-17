
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

#define BREAK if(kbhit()){getch();break;return;}

void error(char* p,int c)
{
fprintf(stderr,"%s\n",p);
if(c!=0) exit(c);
}

void ColorTest(picture& S);
void Test(picture& S);

main()
{
picture* Z=screen::StartMenu();
picture &S=*Z;
int i;
S.MoveTo(100,100);
S.SetColor(15);
S<<10<<" "<<10.55<<endl<<"KONIEC !!!";
for(i=1;i<30;i++)
	S<<"?"<<endl;
S.MoveTo(100,100);
S<<"Rally ?";
S.Clear();
Test(S);
for(i=1;i<=(S.GetMaxY()+1)/2;i++)
	{
	S.ShiftDown(2);
	S.ClearBar(0,0,S.GetMaxX(),2);
	}

S.SetColor(15);
S.SetBackground(1);
S.MoveTo(1,1);
S.OutTextXY(S.GetMaxX()/4,S.GetMaxY()/2-20,"That all folks !");
S.MoveTo(100,100);
S.OutText("The ");
S.OutText("End");
getch();
delete Z;
return 0;
}