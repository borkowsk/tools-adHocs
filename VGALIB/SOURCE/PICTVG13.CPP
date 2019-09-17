/*
Workspace User Interface - metody klasy adaptujacej do VGA w trybie 13h
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

ScreenVGA13::ScreenVGA13()
	{
	SetBase( (pixpointer) MK_FP(0xA000,0x0) ,319,199 );
	int oldmod=peekb(0x0040,0x0049);
	BiosScreen(0x13);   /* Nowy tryb ekranu */
	LockScreen(oldmod); /* Zapamientanie starego trybu */
	SetAspect(::XRATIO,::YRATIO); /* Ustalenie aspektu ekranu */
        SetColor(15);
	}
