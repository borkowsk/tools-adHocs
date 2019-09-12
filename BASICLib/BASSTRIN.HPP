/*========================================================*/
/* 		  BASIC LIKE STRING TYPE                  */
/*========================================================*/

#ifndef __BSTRING_HPP__
#define __BSTRING_HPP__ 1
/*
#include <stdio.h>
#include <stdlib.h>
*/

#include <string.h>

class bstring
{
public:
static
unsigned granularity;
protected:
char*		text;
size_t		len;
unsigned  char	temp/*:8*/;
unsigned  char	stat/*:8*/;
// 	  METODS:
//--------------------------
void cdecl	 clear();		//make empty string (static "")
	 bstring(size_t);	//private constructor
public:
void cdecl fill(size_t n,char c); //fill function
void cdecl erase()		//Truncate string to "" (empty)
		{if(stat) *text='\0'; else clear(); }
friend
char* cdecl   realloc(char* t,size_t n,int stat);
      cdecl   bstring();		//default
      cdecl  ~bstring()			//destructor
		{clear();/*HTEST;*/}
      cdecl   bstring(bstring&);	//copy constructor
      cdecl   bstring(const char*);	//from char* converter
//	 bstring(const char );  	//from single char coverter - but ambiguity


#ifdef _ENABLE_CHAR_OPERATORS_

bstring& cdecl operator =  (const char);
bstring& cdecl operator += (const char);
int      cdecl operator == (const char mt)
		{return (len==1 && *text==mt);}
friend
int      cdecl operator == (const char mt,bstring& wz)
		{return (wz.len==1 && *wz.text==mt);}
int      cdecl operator != (const char mt)
		{return !(len==1 && *text==mt);}
friend
int      cdecl operator != (const char mt,bstring& wz)
		{return !(wz.len==1 && *wz.text==mt);}
friend
bstring  cdecl operator +  (const char z,bstring& wz);
bstring  cdecl operator +  (const  char c);
#endif

	       operator char*()	//to char* converter
		{return text;}
	       operator char()	//to simple char converter
		{return *text;}
int cdecl      operator == (bstring& wz)
		{return (len==wz.len && memcmp(text,wz.text,len)==0 ) ;}
int cdecl      operator == (char* wz)
		{return (strcmp(text,wz)==0) ;}
int cdecl      operator != (bstring& wz)
		{return !(len==wz.len && memcmp(text,wz.text,len)==0 ) ;}
int cdecl      operator != (char* wz)
		{return (strcmp(text,wz)!=0) ;}
bstring& cdecl operator =  (bstring&);
bstring& cdecl operator =  (const char*);
bstring& cdecl operator += (bstring&);
bstring& cdecl operator += (const char*);
bstring cdecl  operator +  (bstring&);
bstring cdecl  operator +  (const char*);

friend
bstring cdecl  operator +  (const char*wztext,bstring& wz);
friend
int cdecl      operator == (char* ch,bstring& wz)
		{return strcmp(ch,wz.text)==0 ;}

size_t cdecl  lastrblank();	//return position of last right blank
size_t cdecl  lastblank();	//return position of last left blank
void cdecl    mid(bstring& expresion, size_t start,size_t lenght=0xffff);
			//replace a portion a string variable with string expresion
void cdecl    lset(bstring& expresion);
			//left justifies the value of a string expression in bstring
void cdecl    rset(bstring& expresion);
			//right justifies the value of a string expression in bstring
size_t cdecl  lenght()	// lenght of string
		  {return len;}
friend
size_t cdecl  len(bstring& ln) // lenght of string
		  {return ln.len;}


//       Functions releted from class bstring
//--------------------------------------------------
friend
bstring cdecl  concat(size_t n,...);
friend
bstring* cdecl addr(bstring& a)
	{ return &a ;}
friend
void cdecl     rfield(FILE* f,size_t n,...);
friend
size_t cdecl   instr(bstring&,bstring&,size_t start=1);
		//searches for a string within another string
friend
bstring cdecl  left(bstring&,size_t  l); //
friend
bstring cdecl  right(bstring&,size_t l); //
friend
bstring cdecl  mid(bstring&  wz,size_t start,size_t length=0xffff);
		//returns a substring of a string
friend
bstring cdecl  ltrim(bstring&);
		//returns a copy of a string with leading spaces removed
friend
bstring cdecl  rtrim(bstring&);
		//returns a copy of a string with trailing spaces removed
friend
bstring cdecl  lcase(bstring&);
		//return a copy of string with all A-Z letters converted to lowercase
friend
bstring cdecl  ucase(bstring&);
		//return a copy of string with all a-z letters converted to uppercase
friend
bstring cdecl  str(double);
		//convert double to bstring
friend
bstring cdecl  hex(long int);
		//convert int to hexadecimal format in bstring
friend
bstring cdecl  oct(long int);
		//convert int to octal format in bstring
friend
double 	cdecl  val(bstring&);
		//convert bstring to double if posible
friend
bstring cdecl  space(size_t len);
		//returns blank string of a specific lenght
friend
bstring cdecl  string(size_t len,char c);
		//returns a string consisting of one repeated characters
friend
bstring cdecl  inkey();
		//returns scancode from keyboard NULL
friend
bstring cdecl  input(size_t n);
		//returns n character from input
friend
bstring cdecl  input(size_t n,FILE* s);
		//returns n character from file
friend
void cdecl     input(FILE*,bstring&);
		//read string from file to "," ";" or "EOL"
friend
void cdecl     input(bstring& b)
		//read string from stdin to "," ";" or "EOL"
		{input(stdin,b);}
friend
void cdecl     line_input(FILE*,bstring&);
		// read string from file to EOL
friend
void cdecl     line_input(bstring& b)
		// read string from file to EOL
		{line_input(stdin,b);}
friend
bstring cdecl  finps(FILE* f);
friend
bstring cdecl  flinps(FILE* f);
friend
bstring cdecl  inps();
friend
bstring cdecl  linps();

/*		for 	basic 	compatybility	 only			 */
/*-----------------------------------------------------------------------*/
friend
bstring cdecl  string(size_t len,bstring& b)
		{return string(len,b.text[0]);}
friend
int cdecl      asc(bstring& b)
		{return b.text[0];}
friend
bstring cdecl  chr(int c);
		//return two character bstring containing char c and '\0'
friend
bstring cdecl  command_line();
		//return entire command line
friend
bstring cdecl  _mk(int i);
friend
bstring cdecl  _mk(long int i);
friend
bstring cdecl  _mk(float i);
friend
bstring cdecl  _mk(double i);
friend
void cdecl     read_data_table(bstring&);
friend
bstring cdecl  curdir(int d);
friend
void cdecl     files(bstring&);
		// Print contens of directory
friend
bstring cdecl  date_s();
		//return bstring contaning date
friend
bstring cdecl  time_s();
		//return bstring contaning time
friend
char* cdecl    using(bstring& s);
		//Convert basic "PRINT USING" format string to C printf format
		//returned value is a eddres of static char[256]
};

inline
char* cdecl realloc(char* t,size_t n,int stat)
//realocation heap string
{
n+=bstring::granularity;
n&=~bstring::granularity;
if( stat ) {char* tt=(char*) malloc(n);
	   if(tt!=NULL)
		memcpy(tt,t,n);
	   return tt;
	   }
    else   return (char*)(::realloc(t,n));
}


#define    MKI(/*integer*/ i) _mk((int)i)
#define    MKL(/*longint*/ i) _mk((long int) i)
#define    MKS(/*float  */ i) _mk((float)i)
#define    MKD(/*double */ i) _mk((double)i)

#define    CVI(/*bstring*/ b) *( (int*)		((char*)(b)) )
#define    CVL(/*bstring*/ b) *( (long int*)	((char*)(b)) )
#define    CVS(/*bstring*/ b) *( (float*)	((char*)(b)) )
#define    CVD(/*bstring*/ b) *( (double*)	((char*)(b)) )

#endif

