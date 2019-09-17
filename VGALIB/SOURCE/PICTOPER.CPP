/*
Workspace User Interface - operatory <<  TTY/Picture
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <values.h>
#include <math.h>
#include <conio.h>

#include "biosgraf.h"
//#include "color.hpp"
#include "picture.hpp"
#include "fonts.hpp"

picture& picture::operator << (int c)
	{
	char bufor[7];
	sprintf(bufor,"%d",c);
	OutText(bufor);
	return *this;
	}

picture& picture::operator << (unsigned c)
	{
	char bufor[7];
	sprintf(bufor,"%u",c);
	OutText(bufor);
	return *this;
	}

picture& picture::operator << (double c)
	{
	char bufor[40];
	sprintf(bufor,"%g",c);
	OutText(bufor);
	return *this;
	}

picture& picture::operator << (const unsigned char* t)
	{
	OutText((char*)t);
	return *this;
	}

picture& picture::operator << (const signed char* t)
	{
	OutText((char*)t);
	return *this;
	}

picture& picture::operator << (signed char c)
	{
        char t[2];
        t[0]=(char)c;
        t[1]='\0';
	OutText(t);
	return *this;
	}

picture& picture::operator << (unsigned char c)
	{
        char t[2];
        t[0]=(char)c;
        t[1]='\0';
	OutText(t);
	return *this;
	}

