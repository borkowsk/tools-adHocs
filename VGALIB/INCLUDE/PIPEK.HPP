#include <stdio.h>
#include <conio.h>
class Interface
{
protected:
static int key;
static unsigned int IsMouse;
static int mosX,mosY;
static int mosClik;
static gdriver;
public:
static int  GetLastKey(){return key;}
static int  GetMosX(){return mosX;}
static int  GetMosY(){return mosY;}
static int  GetClik(){return mosClik;}
static int  GetIsMouse(){return IsMouse;}
static int  MousePresent();
static int  CheckClik();
static char Getcomm();
static void VisCursor();
static void UnVisCursor();
};

/*
class pipek : Interface
{
int x,y,xd,yd;
int color,ramka;
int keychar,cliksens;
char* text;
public:
static void clear();
pipek();
int DoIt();
void InstallText(const char* t,int lkey,int lsens);
void set(int lx,int ly,int lxd, int lyd, int lcol , int lram);
void put();
void invers();
};
*/