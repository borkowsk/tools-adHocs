#ifndef _INC_MOUSE_HPP
#define _INC_MOUSE_HPP
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
static int  GetClick(){return mosClik;}
static int  GetIsMouse(){return IsMouse;}
static int  MousePresent();
static int  CheckClick();
static char Getcomm();
static void VisCursor();
static void UnVisCursor();
static void MouseBox(int MinX,int MinY,int MaxX,int MaxY);
static void SetCursorPos(int x,int y);
};
#endif
