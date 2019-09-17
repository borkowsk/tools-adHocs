/*
Biblioteka Workspace User Interface - klasa okna w obrazku
*/
class Window : public picture
{

void PlotPixel(pixsize_t x,pixsize_t y)
{
pixsize_t x1=StartX+x;
pixsize_t y1=StartY+y;
*(GetTarget().CalcPixelAddr(x1,y1))=GetColor();
}

pixpointer CalcPixelAddr(pixsize_t& x,pixsize_t& y) const
{
x+=StartX;
y+=StartY;
pixpointer pom=GetTarget().CalcPixelAddr(x,y);
x-=StartX;
y-=StartY;
if(x>GetMaxX())
	x=GetMaxX();
if(y>GetMaxY())
	y=GetMaxY();
return pom;
}

protected:
Window():picture()
       {	}
public:
virtual void ToTarget()
	{ GetTarget().ToTarget();  }
Window(picture& ntarget,pixsize_t ANHORX,pixsize_t ANHORY,pixsize_t SMAXX,pixsize_t SMAXY): picture()
	{
	SetBase(NULL,SMAXX,SMAXY);
	SetTarget(ntarget);
	SetAnhore(ANHORX,ANHORY);
	SetFont(ntarget.GetFont());
	SetPalette(ntarget.GetPalette());
	SetCopyMode(ntarget.GetCopyMode());
	SetColor(ntarget.GetColor());
	SetBackground(ntarget.GetBackground());
	SetAspect(ntarget.GetXRatio(),ntarget.GetYRatio());
	}
};


