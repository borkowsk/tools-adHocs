/*
protoplasta typu picture
*/
#include "color.hpp"

typedef char far* pixpointer;
typedef unsigned pixsize_t;

class pixrec
{
// PUBLIC ENUMERATION TYPES
public:
enum copymode { COPYPUT , TRANSPARENTPUT , XORPUT }   ;
// PRIVATE DATA
private:
pixrec*    target;   // Target picture or NULL
pixpointer  bufer;
/*rgb_palette* palet; */
color CUR_COLOR;
pixsize_t CUR_X,CUR_Y;
copymode CUR_MODE;
void (*colorlinecopy)(pixpointer Dest,pixpointer Sour,pixsize_t N);

pixsize_t StartX,StartY;
pixsize_t MAXX,MAXY;
pixsize_t XRATIO,YRATIO;

// METODY POMOCNICZE
	// Ustawianie parametrow w konstruktorach
void AllDefault();
	// Prywatne drukowanie pixela
virtual void PlotPixel(pixsize_t x,pixsize_t y);
	// Prywatne obliczanie adresu - wartosci x,y po wyjsciu wskazuja
	// najdalszy dostepny w tym adresie punkt.
virtual pixpointer CalcPixelAdrr(pixsize_t& x,pixsize_t& y);

// INTERFACE :
public:
	//Konstruktor bezparametrowy robi pusty,niezwiazany objekt
picture() { AllDefault(); }
	// Konstruktor wiazacy z
picture(void* framebuffer,unsigned SMAXX,unsigned SMAXY);
	  { AllDefault();bufer=framebufer;MAXX=SMAXX;MAXY=SMAXY;}
	//Destruktor virtualny
virtual ~picture();
// GET PARAMETERS
long GetMaxX() { return MAXX; }
long GetMaxY() { return MAXY; }
copymode GetCopyMode() { return CUR_MODE; }
// SET PARAMETERS
int SetCopyMode(copymode m); // Set mode for copy pixel to this picture
int SetAspect(pixsize_t NXRATIO,pixsize_t NYRATIO);
int SetColor(color c);   // Set current color
int SetColor(unsigned R,unsigned G,unsigned B); // Set current colorfrom RGB
int MoveTo(long x,long y);
int MoveRel(long x,long y);
// CLEARING
int Clear();
int ClearBar(long x1,long y1,long x2,long y2);
// PAINTING
int PutPixel(long x,long y,color c);     	// Put pixel with color c
int FillLine(long x,long y,long n);		// Fill horizontal line
int CopyLine(long x,long y,long n,const color* src);// Copy horizontal line
int CopyPicture(long x,long y, const pixrec& sou); // Copy another picture at x,y
int FlowPicture(const picture& sou);            // Copy another picture at 0,0
// DRAWING
int DrawLine(long x1,long y1,long x2,long y2);
int DrawLineTo(long x,long y);
int DrawLineRel(long x,long y);
int DrawRectAngle(long x1,long y1,long x2,long y2);
int DrawCircle(long x,long y,long r);
int DrawEllipse(long x,long y,long a,long b);
int FillRectAngle(long x1,long y1,long x2,long y2);
int FillCircle(long x,long y,long r);
int FillEllipse(long x,long y,long a,long b);
};
/*
void picture::AllDefault()
{
target=NULL;   // Target picture or NULL
bufer=NULL;
/*rgb_palette* palet; */
CUR_COLOR.free(); // Free color and set to 0 - transparent
CUR_X=CUR_Y=0;
CUR_MODE=TRANSPARENTPUT;
colorlinecopy=NULL;
StartX=StartY=0;
MAXX=MAXY=0;
XRATIO=YRATIO=1;
}
*/