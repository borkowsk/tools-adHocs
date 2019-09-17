/*
Test WUI - procedury obdslugi ekranu
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

#define BREAK if(kbhit()){getch();break;}

void ColorTest(picture& );
void Test(picture& );

main()
{
picture *Z=screen::StartMenu();
Window W(*Z,10,10,Z->GetMaxX()/2,Z->GetMaxY()/2);
int i;
randomize();
Z->Clear();
Z->MoveTo(100,100);
Z->SetColor(15);
W.SetBackground(10);
W.Clear();
W.SetBackground(1);
for(i=1;i<=(W.GetMaxY()+1)/2;i++)
	{
	W.PutPixel(random(W.GetMaxX()),random(W.GetMaxY()),random(256));
	}
W.SetColor(4);
W.SetBackground(1);
for(i=1;i<=(W.GetMaxY()+1)/2;i++)
	{
	W.ShiftDown(2);
	W.ShiftLeft(2);
	W.ClearBar(W.GetMaxX()-2,0,W.GetMaxX(),W.GetMaxY());
	W.ClearBar(0,0,W.GetMaxX(),i*2);
	BREAK;
	}
W.MoveTo(W.GetMaxX()/3,W.GetMaxY()/2);
W<<"WINDOW"<<endl;
W.SetBackground(2);
for(i=1;i<=(W.GetMaxX()+1)/2;i++)
	{
	W.ShiftRight(2);
	W.ClearBar(0,0,2,W.GetMaxY());
	BREAK;
	}
W.MoveTo(W.GetMaxX()/2,W.GetMaxY()/2);
for(i=1;i<(W.GetMaxX()/16);i++)
	{
	W<<"?"<<endl;
	BREAK;
	}
Z->MoveTo(Z->GetMaxX()/2,Z->GetMaxY()/2);
(*Z)<<10<<" "<<10.55<<endl<<"KONIEC !!!";
for(i=1;i<(Z->GetMaxX()/16);i++)
	{
	(*Z)<<"?"<<endl;
	BREAK;
	}
Z->MoveTo(100,100);
(*Z)<<"Rally ?";
Z->Clear();
Test(*Z);
Z->SetColor(15);
Z->SetBackground(0);
Z->MoveTo(1,1);
Z->OutTextXY(Z->GetMaxX()/4,Z->GetMaxY()/2-20,"That's all, folks!");
Z->MoveTo(100,100);
Z->SetColor(4);
Z->OutText("The ");
Z->OutText("End");
getch();
delete Z;
return 0;
}


