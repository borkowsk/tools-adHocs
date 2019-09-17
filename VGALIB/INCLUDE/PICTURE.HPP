/*
Bibliotek Workspace User Interface
deklaracje glownych klasy obslugi ekranu
*/
#ifndef _PICTURE_HPP_
#define _PICTURE_HPP_

//#include <stdlib.h>
//#include <string.h>
#include <mem.h>

#ifdef USE16BITONLY
typedef unsigned      pixsize_t;
typedef int           pixco_t;
#else
typedef unsigned long pixsize_t;
typedef long          pixco_t;
#endif

typedef unsigned char pixel;
typedef pixel far* pixpointer;
typedef pixco_t       fullpixco_t;
typedef struct{pixco_t x,y;} xypoint;
typedef unsigned char color_handle;
typedef unsigned char byte;
typedef unsigned int  word;
#define MAXXYCO  MAXINT
#define MINXYCO -MAXINT

/*
void movedata(unsigned srcseg, unsigned srcoff, unsigned destseg,unsigned destoff, size_t n);
void far *far _fmemset (void far *s, int c, size_t n);
void far *far _fmemcpy(void far *dest, const void far *src, size_t n);
*/
#define MEMSET _fmemset
#define MEMCPY _fmemcpy
#define PROT   /*const*/
#define min(_A_,_B_) ((_A_)<(_B_)?(_A_):(_B_))
void translinecopy(pixpointer  p,pixpointer b,unsigned len );

class font;
class picture;
class mem_picture;
class screen;

struct fontinfo
{
font* f;
int   stat;
};

struct RGB
{
unsigned char R,G,B;
RGB()
	{}
RGB(RGB& W)
	{R=W.R;G=W.G;B=W.B;}
RGB(unsigned char r,unsigned char g,unsigned char b)
	{R=r;G=g;B=b;}
};

class palette
{
friend class picture;
friend class screen;
private:
RGB*  nodes;/* tablica RBG dla N kolorow */
byte* locks;/* tablica blokowania dla N kolorow */
unsigned  N;/* ilosc kolorow w palecie */
unsigned  S;/* pierwszy kolor */
unsigned  not_lock;
byte flag;
byte   cubea;
byte   cubes;
protected:
static void palette::Mask(RGB& w);
static void palette::UnMask(RGB& w);
void fromhardware();/* Odczytanie rejestrow kolorow karty */
void tohardware();  /* Uzgodnienie tablicy kolorow karty z wirtualna */
void tohardware(color_handle); /* Uzgodnienie jednego koloru karty z paleta */
void direct(byte d) /* Ustalenie natychmiastowego uzgadniania(1) lub opoznionego */
	{flag=d;}
void cube(int n);   /* Wpisanie na koncu tablicy RBG kostki kolorow */
void grayscale(int n); /* Wpisanie na koncu tablicy skali szarosci */
public:
     palette(unsigned LN,unsigned LS); /* Konstruktor palety o N kolorach, startujacy os S */
    ~palette();
void Lock(color_handle);   /* Zablokowanie koloru przed zmianami */
void Unlock(color_handle); /* Odblokowanie koloru */
void Free(color_handle);   /* To samo - do pary z Alloc   */
virtual
unsigned ColorDist(RGB&,RGB&); /* Odleglosc miedzy kolorami */
RGB  Values(color_handle); /* Wartosci RGB zwiazane z uchwytem koloru */
color_handle Alloc(RGB ,unsigned th);/* Uchwyt koloru nie dalszego niz th od zadanego lub 0 (brak miejsca) */
color_handle FromCube( RGB); /* Uchwyt koloru z kostki kolorow */
};

class graypallette:public palette
{
graypallette(unsigned GLN,unsigned GLS):palette(GLN,GLS)
        { grayscale(GLN); }
~graypallette() {}
};

class font
{
friend class picture;
private:
static fontinfo FontTypes[];
static size_t NumberOfTypes;
static size_t CurentNumber;
virtual int OutTextXY(picture& p,pixco_t x,pixco_t y,const char* t)=0;//Tylko dla picture
public:
static size_t AvailableFonts()	{ return CurentNumber; }
static int  RegisterFont(font* f,int stat);
static font*  FontByNumber(size_t N);
friend void  ExitFonts();   // At Exit function - free resorces
virtual pixsize_t TextWidth(const char* t)=0;
virtual pixsize_t TextHight(const char* t)=0;
};

class picture
{
friend class font;
// PUBLIC ENUMERATION TYPES
public:
enum copymode { COPYPUT , TRANSPARENTPUT , XORPUT }   ;

// PRIVATE DATA
private:
pixpointer       bufer;
pixel*      linebuf;
picture*    target;   // Target picture or NULL
font*       curfont;
palette*    palet;
void (*colorlinecopy)(pixpointer Dest,pixpointer Sour,unsigned N);

protected:
pixco_t		MAXX,MAXY;
copymode 	CUR_MODE;
color_handle 	CUR_COLOR;
color_handle 	BACKGROUND;
pixco_t 	CUR_X,CUR_Y;
pixco_t 	StartX,StartY;

pixsize_t LINELEN;
pixsize_t XRATIO,YRATIO;

// METODY POMOCNICZE
protected:
pixpointer GetFrameBuferAddr() const { return bufer; }
	// Ustawianie parametrow w konstruktorach
void AllDefault() ;
void SetBase(pixpointer framebuffer,
	     unsigned SMAXX,
	     unsigned SMAXY) PROT ;
// HIDE INTERFACE
public:
virtual void PlotPixel(pixsize_t x,	// Prywatne drukowanie pixela
		       pixsize_t y )=0;
virtual pixpointer CalcPixelAddr(pixsize_t& x,
				 pixsize_t& y) const // Prywatne obliczanie adresu - wartosci x,y po wyjsciu wskazuja
		       { x=0;y=0;return NULL; }
					// najdalszy dostepny w tym adresie punkt.
picture(){ AllDefault(); }//Konstruktor bezparametrowy robi pusty,niezwiazany objekt
virtual
~picture(){if(linebuf!=NULL)
		delete linebuf;
	   AllDefault(); }//Destruktor virtualny
// GET PARAMETERS
pixsize_t GetLineLen() const { return LINELEN;}
pixel*    GetLineBuf() const { return linebuf;}
pixco_t GetMaxX() const  { return MAXX;   }
pixco_t GetMaxY() const  { return MAXY;   }
pixco_t GetX() const  { return StartX;   }
pixco_t GetY() const  { return StartY;   }
pixsize_t GetXRatio() const  { return XRATIO; }
pixsize_t GetYRatio() const  { return YRATIO; }
color_handle GetColor() const      { return CUR_COLOR;  }
color_handle GetBackground() const { return BACKGROUND; }
picture& GetTarget() const   { return *target;   }
font&    GetFont()   const   { return *curfont;  }
palette& GetPalette() const  { return *palet;    }
copymode GetCopyMode() const { return CUR_MODE;  }
pixpointer PixelAddr(pixco_t& x,pixco_t& y) const;
// SET PARAMETERS
int SetTarget(picture& T);
int SetAnhore(pixco_t x,pixco_t y);
int SetFont(font& f);
int SetPalette(palette& p)
	{palet=&p;return 1;}
int SetCopyMode(copymode m); // Set mode for copy pixel to this picture
int SetAspect(pixsize_t NXRATIO,pixsize_t NYRATIO)
	{ XRATIO=NXRATIO;YRATIO=NYRATIO; return 1;}
int SetColor(color_handle c)   // Set current color
	{CUR_COLOR=c;return 1;}
int SetBackground(color_handle b)
	{ BACKGROUND=b;return 1; }
void LockColor()
	{ palet->Lock(CUR_COLOR); }
void LockBackground()
	{ palet->Lock(BACKGROUND);}
void UnLockColor()
	{ palet->Unlock(CUR_COLOR); }
void UnLockBackground()
	{ palet->Unlock(BACKGROUND);}
int MoveTo(pixco_t x,pixco_t y)
	{ CUR_X=x;CUR_Y=y;return 1;}
int MoveRel(pixco_t x,pixco_t y)
	{ CUR_X+=x;CUR_Y+=y;return 1;}

// FULL PICTURE OPERATION
virtual int Clear();
virtual int Fill(color_handle c);
virtual int ShiftUp(pixco_t);
virtual int ShiftDown(pixco_t);
virtual int ShiftLeft(pixco_t);
virtual int ShiftRight(pixco_t);

// SENDING
virtual void ToTarget()=0;

// PAINTING
int ClearBar(pixco_t x1,pixco_t y1,pixco_t x2,pixco_t y2);
int PutPixel(pixco_t x,pixco_t y,color_handle c);     	    // Put pixel with color c
color_handle  GetPixel(pixco_t x,pixco_t y);     	    // Get pixel with from x,y
int FillLine(pixco_t /*x*/,pixco_t /*y*/,pixco_t /*n*/);	    // Fill horizontal line
int  PutLine(pixco_t /*x*/,pixco_t /*y*/,pixco_t /*n*/,const pixpointer src);// Copy horizontal line
int GetLine(pixpointer c_tar,pixco_t /*x*/,pixco_t /*y*/,pixco_t /*n*/) const ;// Copy line from screen to bufor
virtual int ShiftLine(pixco_t x,pixco_t y,pixco_t shval); // Shift line
virtual int PutPicture(pixco_t ,pixco_t , const mem_picture& ); // Copy another picture at x,y
virtual int FlowPicture(const mem_picture& what );         // Copy another, same LINELEN picture at 0,0

// DRAWING
int DrawLine(pixco_t x1,pixco_t y1,pixco_t x2,pixco_t y2);
int DrawLineTo(pixco_t x,pixco_t y);
int DrawLineRel(pixco_t x,pixco_t y);
int DrawRectAngle(pixco_t x1,pixco_t y1,pixco_t x2,pixco_t y2);
int DrawCircle(pixco_t x,pixco_t y,pixco_t r);
int DrawEllipse(pixco_t x,pixco_t y,pixco_t a,pixco_t b);
int DrawPoly(xypoint* ,size_t n);	// Draw poly-line
int FillRectAngle(pixco_t x1,pixco_t y1,pixco_t x2,pixco_t y2);
int FillCircle(pixco_t x,pixco_t y,pixco_t r);
int FillEllipse(pixco_t x,pixco_t y,pixco_t a,pixco_t b);
int FillPoly(xypoint* ,size_t n);	// Fill polygon

// TEXT OUTPUT
int OutTextXY(pixco_t x,pixco_t y,const char* t); // Draw text at x,y
int OutText(const char* t);                               // Draw text at curent position and move cur-pos

// STREAM LIKE OPERATORS & MANIPULATORS
private:
// Formating fields
/*  	* * *  		 */
public:
// insert the character
picture&   operator<< (  signed char);
picture&   operator<< (unsigned char);

// for the following, insert character representation of numeric value
picture&   operator<< (short);
picture&   operator<< (unsigned short);
picture&   operator<< (int);
picture&   operator<< (unsigned int);
picture&   operator<< (long);
picture&   operator<< (unsigned long);
picture&   operator<< (float);
picture&   operator<< (double);
picture&   operator<< (long double);

// insert the null-terminated string
picture&   operator<< (const   signed char _FAR *);
picture&   operator<< (const unsigned char _FAR *);

// insert character representation of the value of the pointer
picture&   operator<< (void _FAR *);

// insert manipulator - for manipulators only
picture&  operator<< (picture& (*_f)(picture&))
		{ return (*_f)(*this); }

// manipulators
friend
picture&   endl(picture&); // insert newline and flush
friend
picture&   flush(picture&);// flush the picture
friend
picture&       dec(picture&);      // set conversion base to decimal
friend
picture&       hex(picture&);      // set conversion base to hexidecimal
friend
picture&       oct(picture&);      // set conversion base to octal
};

class mem_picture: public picture
{
friend class picture;//udostepnienie atrybutow tej klasy klasie maciezystej
int static_mem; // Informacja ze nie nalezy zwalniac pamieci
void PlotPixel(pixsize_t x,pixsize_t y);
pixpointer CalcPixelAddr(pixsize_t& x,pixsize_t& y) const;
protected:
mem_picture():picture()
	{}
public:
int GetPicture(const picture& ,pixco_t ,pixco_t );  // Copy from another picture
void ToTarget()
	{
	GetTarget().PutPicture(StartX,StartY,*this);
	}
int ShiftUp(pixco_t);
int ShiftDown(pixco_t);
int ShiftLine(pixco_t ,pixco_t ,pixco_t /*shval*/); // Shift line
mem_picture(pixpointer framebuffer,pixsize_t SMAXX,pixsize_t SMAXY)
	{
	SetBase(framebuffer,SMAXX,SMAXY);
	static_mem=1;
	}

mem_picture(pixsize_t SMAXX,pixsize_t SMAXY)
	{
	static_mem=1;
	unsigned long size=(SMAXX+1)*(SMAXY+1);
	if(size<(size_t)0xffffffffL)
		{
		pixpointer frame=new pixel[size];
		if(frame!=NULL)
			{
			SetBase(frame,SMAXX,SMAXY);
			static_mem=0;
			}
		}

	}

~mem_picture()
	{
	if(static_mem==0)
		delete  GetFrameBuferAddr();
	}
};


struct screeninfo
{
char* names;
picture* (*make)();
};

class screen
{
static screeninfo ScreenTypes[];
static size_t NumberOfTypes;
static int OldMode;
protected:
static int InGraph;
static void LockScreen(int l_oldmod)
	{
	InGraph=1;
	if(l_oldmod!=0)
		OldMode=l_oldmod;
		else
		OldMode=3;
	}

static int FreeScreen()
	{
	if(InGraph!=0)
		{InGraph=0;
		 return OldMode;}
		 else
		 return -1;
	}
public:
static picture* StartMenu();
static int IsInGraph() { return InGraph; }
};

inline int picture::PutPixel(pixco_t x,pixco_t y,color_handle c)
	    // Put pixel with color c
{
if(x>=0 && x<=MAXX && y>=0 && y<=MAXY )
	{
	CUR_COLOR=c;
	PlotPixel((pixco_t)x,(pixco_t)y);
	return 1;
	}
return 0;
}

inline int picture::DrawLineTo(pixco_t x,pixco_t y)
	  {
	  int pom=DrawLine(CUR_X,CUR_Y,x,y);
	  CUR_X=x;CUR_Y=y;
	  return pom;
	  }

inline int picture::DrawLineRel(pixco_t x,pixco_t y)
	  {
	  int pom=DrawLine(CUR_X,CUR_Y,CUR_X+x,CUR_Y+y);
	  CUR_X+=x;CUR_Y+=y;
	  return pom;
	  }

inline int picture::ClearBar(pixco_t x1,pixco_t y1,pixco_t x2,pixco_t y2)
	  {
	  color_handle oldcolor=CUR_COLOR;
	  CUR_COLOR=BACKGROUND;
	  int co=FillRectAngle(x1,y1,x2,y2);
	  CUR_COLOR=oldcolor;
	  return co;
	  }

inline int picture::FillCircle(pixco_t xxs,pixco_t yys,pixco_t bb)
	  {
	  long aa=bb*XRATIO/YRATIO;
	  if(aa==0) aa=bb;
	  return FillEllipse(xxs,yys,bb,aa);
	  }

inline int  picture::DrawCircle(pixco_t  xxs,pixco_t  yys,pixco_t  bb)
	  /* narysowanie okregu  */
	  {
	  long aa=bb*XRATIO/YRATIO;
	  if(aa==0) aa=bb;
	  return DrawEllipse(xxs,yys,bb,aa);
	  }

inline void picture::SetBase(pixpointer framebuffer,unsigned SMAXX,unsigned SMAXY) PROT
	  {
	  *(&bufer)=framebuffer;
	  *(&MAXX)=SMAXX;
	  *(&MAXY)=SMAXY;
	  *(&LINELEN)=MAXX+1;
	  linebuf= new pixel[LINELEN];
	  }

inline  int  font::RegisterFont(font* f,int stat)
	{
	if(CurentNumber<NumberOfTypes)
		{
		FontTypes[CurentNumber].f=f;
		FontTypes[CurentNumber].stat=stat;
		CurentNumber++;
		return CurentNumber+1;
		}
	return 0;
	}

inline  font*  font::FontByNumber(size_t N)
	{
	if(N<= NumberOfTypes) return FontTypes[N].f;
			      return NULL;
	}

inline int picture::OutTextXY(pixco_t x,pixco_t y,const char* t) // Draw text at x,y
	{
	pixco_t W=curfont->TextWidth(t);
	pixco_t H=curfont->TextHight(t);
	if(x+W>=0 && x<=MAXX && y+H>=0 && y<=MAXY)
		return curfont->OutTextXY(*this,x,y,t);
	return 0;
	}

inline int picture::OutText(const char* t)                               // Draw text at curent position and move cur-pos
	{
	pixco_t W=curfont->TextWidth(t);
	pixco_t H=curfont->TextHight(t);
	int pom=0;
	if(CUR_X+W>=0 && CUR_X<=MAXX && CUR_Y+H>=0 && CUR_Y<=MAXY)
		pom= curfont->OutTextXY(*this,CUR_X,CUR_Y,t);
	CUR_X+=W;
	return pom;
	}

inline picture&   endl(picture& p)
	// insert newline and shift if requaried
	{
	pixco_t s=p.curfont->TextHight("Xg");
	p.CUR_Y+=s;
	p.CUR_X=0;
	if(p.CUR_Y>p.MAXY)
		{
		p.ShiftUp(s);
		p.ClearBar(0,MAXY-s,MAXX,MAXY);
		}
	return p;
	}

inline int picture::SetTarget(picture& T)
	{
	XRATIO=T.XRATIO;
	YRATIO=T.YRATIO;
	target=&T;
	palet=&T.GetPalette(); // Palety musza byc zgodne
	StartX=StartY=0;
	return 1;
	}

inline int picture::SetAnhore(pixco_t x,pixco_t y)
	{
	StartX=x;StartY=y;
	if(x>=0 && x<=MAXX && y>=0 && y<=MAXY)
		return 1;
		else
		return 0;
	}

inline int picture::SetFont(font& f)
	{
	curfont=&f;
	return 1;
	}

#endif