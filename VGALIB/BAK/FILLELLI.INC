
int picture::FillEllipse(long int xxs,long int yys,long int bb,long int aa)
{
register int xxp,yyp;
register int xs=xxs;
register int ys=yys;
register int yi=0;
register int xi=0;
register int n=0;

long int a2=aa*aa;
long int b2=bb*bb;
long int a2s=a2;
long int b2s=b2*(2*aa+1);
long int DELTAi=2*(1-bb);

if(aa==0 || bb==0)
		return 0;
color_handle COL_CORE=GetColor();
RGB rgblocal=palet->Values(COL_CORE);
rgblocal.R/=2;rgblocal.G/=2;rgblocal.B/=2;
color_handle COL_BORDER=palet->Alloc(rgblocal,1);

if(aa<-MAXINT) yi=-MAXINT;
   else if(aa>MAXINT) yi=MAXINT;
	    else   yi=(pixco_t) aa;
yyp=MAXINT;
while( yi>=0 )
{
xxp=xs-xi;
if(yyp!=yi) // Nowa linia
	{
	n=2*xi+1;
	yyp= ys-yi;
	FillLine(xxp,yyp,n);
	yyp= ys+yi;
	FillLine(xxp,yyp,n);
	}
	else // Uzupelnienie lini o punkty brzezne
	{
	SetColor(COL_BORDER);
	xxp=xs+xi;  yyp= ys+yi;
	if(xxp>=0 && xxp<=MAXX && yyp>=0 && yyp<=MAXY )
		PlotPixel(xxp , yyp);
	xxp=xs-xi;  yyp= ys-yi;
	if(xxp>=0 && xxp<=MAXX && yyp>=0 && yyp<=MAXY )
		PlotPixel(xxp , yyp);
	xxp=xs+xi;  yyp= ys-yi;
	if(xxp>=0 && xxp<=MAXX && yyp>=0 && yyp<=MAXY )
		PlotPixel(xxp , yyp);
	xxp=xs-xi;  yyp= ys+yi;
	if(xxp>=0 && xxp<=MAXX && yyp>=0 && yyp<=MAXY )
		PlotPixel(xxp , yyp);
	SetColor(COL_CORE);
	}
yyp=yi; // zapaminetaj do porownania
BEZRYSOWANIA:
if(DELTAi<0L) goto _1300;
   else {if(DELTAi==0L)
		goto _1240;
	  else
		goto _1180; }

_1180:			 /* decyzja */
if( (DELTAi+DELTAi-a2s)<=0L )
		goto _1240;
	else
		goto _1380;
//continue;

_1240: 			/* krok ukosny */
xi++;
yi--;
a2s+=a2+a2;
b2s-=(b2+b2);
DELTAi+=(a2s-b2s);
continue;

_1300:			/* krok poziomy */
if((DELTAi+DELTAi+b2s)>0L) goto _1240;
xi++;
a2s+=a2+a2;
DELTAi+=a2s;
continue;
//goto BEZRYSOWANIA;

_1380:			/* krok pionowy */
yi--;
b2s-=(b2+b2);
DELTAi-=b2s;
}

return 1;
}
