
#define uchar      unsigned char
void GrayCorection1()
{
int i;
struct palettetype pal;
struct RGB { uchar red; uchar green; uchar blue; } k[]={
{0x00,0x00,0x00},
{0x1*4,0x1*4,0x1*4},
{0x2*4,0x2*4,0x2*4},
{0x3*4,0x3*4,0x3*4},
{0x4*4,0x4*4,0x4*4},
{0x5*4,0x5*4,0x5*4},
{0x6*4,0x6*4,0x6*4},
{0x7*4,0x7*4,0x7*4},
{0x8*4,0x8*4,0x8*4},
{0x9*4,0x9*4,0x9*4},
{0xA*4,0xA*4,0xA*4},
{0xB*4,0xB*4,0xB*4},
{0xC*4,0xC*4,0xC*4},
{0xD*4,0xD*4,0xD*4},
{0xE*4,0xE*4,0xE*4},
{0xF*4,0xF*4,0xF*4}
};
for(i=0; i<16; i++)
	{
	setfillstyle(SOLID_FILL, i );
	bar(i*40,0,(i+1)*40,getmaxy());
	}
/* grab a copy of the palette */
getpalette(&pal);
/* create gray scale */
for (i=0; i<pal.size; i++)
      setrgbpalette(pal.colors[i], k[i].red , k[i].green , k[i].blue);
getch();
}




void setVGApalette(int i ,uchar red,uchar green, uchar blue )
{
_BX=i;
_CH=green;
_CL=blue;
_DH=red;
_AL=0x10;// podfunkcja 10H
_AH=0x10;// funkcja 10H "kolory i atrybuty"
geninterrupt(0x10);
}

void GrayCorection0()
{
 struct palettetype pal;
 int i,xmax,ht,y;
/* grab a copy of the palette */
getpalette(&pal);

/* create gray scale */
for (i=0; i<pal.size; i++)
      setrgbpalette(pal.colors[i], i*4, i*4, i*4);

/* display the gray scale */
   ht = getmaxy() / 16;
   xmax = getmaxx();
   y = 0;
   for (i=0; i<pal.size; i++)
   {
      setfillstyle(SOLID_FILL, i);
      bar(0, y, xmax, y+ht);
      y += ht;
   }

getch();
}