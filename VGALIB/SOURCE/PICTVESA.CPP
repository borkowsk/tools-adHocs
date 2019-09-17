/*
Worspace User Interface - inicjacja ekranu typu "VESA 256 color"
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <values.h>
#include <mem.h>
#include <dos.h>

#include "biosgraf.h"
#include "color.hpp"
#include "picture.hpp"
#include "fonts.hpp"
#include "screens.hpp"
#include "vesa.hpp"

picture* ScreenVESA::StartVESA() /* Dynamiczna alokacja deskryptora ekranu */
	{
	VESAinfo b;
	if(InGraph==0 && IsVesa(&b) )
		return new ScreenVESA;
		else
		return NULL;
	}

ScreenVESA::ScreenVESA()
	{
	VESAinfo b;
	int newmode=0xffff;
	int oldmod=peekb(0x0040,0x0049);

	/* Wyswietlenie trybow pracy */
       if(IsVesa(&b))
	{
	printf("Bios extension:%c%c%c%c %u %Fs \nAvailable modes:",b.Syg[0],b.Syg[1],b.Syg[2],b.Syg[3],b.Nr,b.NazwaProd);
	int i=0;
	while(b.ListaTr[i]!=0xffff){ printf("%xH ",b.ListaTr[i]); i++;}
	}
	else printf("\t\t\tVESA bios extension not detected\n");

	{
	char tab[100];
	char* end;
	printf("Press ENTER");
	gets( tab );
	printf("Mode ? :");
	gets( tab );
	newmode=strtol( tab, &end , 0 );
	printf(" Mode %d \n czyli %xH", newmode , newmode  );
	}

	LockScreen(oldmod); /* Zapamientanie starego trybu */
	BiosScreen(newmode);   /* Nowy tryb ekranu */
	SetBase( (pixpointer) MK_FP(0xA000,0x0) ,::MAXX,::MAXY );
	SetAspect(::XRATIO,::YRATIO); /* Ustalenie aspektu ekranu */
        SetColor(15);
	}
