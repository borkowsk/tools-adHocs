/*
Workspace User Interface - deklaracja klasy MouseDriver
*/

union  MouseMask;
struct MouseEvent;
class  MouseDriver;
typedef void (far* MouseIntRoutine)();


union MouseMask
{
unsigned all;
struct {
unsigned CursorMoved:1;
unsigned LeftPush:1;
unsigned LeftRelease:1;
unsigned RightPush:1;
unsigned RightRelease:1;
unsigned ShiftActive:1;
unsigned CtrlActive:1;
unsigned AltActive:1;
unsigned Reserved:8;
         } s;
void Null()
{all=0;}
void SetMask(int CM,int LP,int LR,int RP,int RR,int SA,int CA,int AA)
{s.CursorMoved=CM;
s.LeftPush=LP;s.LeftRelease=LR;
s.RightPush=RP;s.RightRelease=RR;
s.ShiftActive=SA;s.CtrlActive=CA;s.AltActive=AA;s.Reserved=0;}
operator unsigned ()
{ return all; }
void FromUnsigned(unsigned V)
{ all=V; }
MouseMask(unsigned V)
{ all=V; }
MouseMask()
{ all=0; }
};

struct MouseEvent
{
MouseMask Mask;
unsigned MosClick;
int MosX;
int MosY;
int ImpX;
int ImpY;
MouseEvent()
{FromRegs();}
void FromRegs()
{Mask.FromUnsigned(_AX);MosClick=_BX;MosX=_CX;MosY=_DX;ImpX=_SI;ImpY=_DI;}
};

class MouseDriver
{
static void DefaultMouseRoutine();
protected:
static unsigned   IsMouse;
static MouseEvent GlEvt;
/*?*/  MouseEvent DefEvt;
static int        hardware;
void far* StateBuffer;
public:
// Akcesory
/*?*/ void GetLastEvent(MouseEvent& Event) {disable();Event=GlEvt;enable();}
/*?*/ int  GetMosX(){return DefEvt.MosX;}
/*?*/ int  GetMosY(){return DefEvt.MosY;}
/*?*/ int  GetClik(){return DefEvt.MosClick;}
/*?*/ int  GetIsMouse(){return IsMouse;}
// Konstruktor i destruktor
MouseDriver() { StateBuffer=NULL;}
~MouseDriver() { if(StateBuffer!=NULL) delete StateBuffer; }
// Implementacja funkcji
/*?*/ int  Init(int nhardware);
/*?*/ void ShowCursor();
/*?*/ void HideCursor();
/*?*/ int  CheckStatus();
/*?*/ void SetMouseCursorPosition(int X,int Y);
/*?*/ unsigned GetButtonPressInformation(int button,int& X,int& Y);
/*?*/ unsigned GetButtonReleaseInformation(int button,int& X,int& Y);
/*?*/ void SetMinMaxHorizontalPosition(int minx,int maxx);
/*?*/ void SetMinMaxVerticalPosition(int miny,int maxy);
/*?*/ void SetGraphicsCursorBlock(int hotX,int hotY,void far* maskblock);
/*?*/ void SetTextCursor(int hardSoft,unsigned mask1,unsigned mask2);
/*?*/ void ReadMouseMotionCounters(int& motX,int& motY);
/*?*/ void SetInterruptSubrutine(void far* address,MouseMask Mask);
/*?*/ void LightPenEmulationOn();
/*?*/ void LightPenEmulationOff();
/*?*/ void SetMickeyPixelRatio(unsigned RX,unsigned RY);
/*?*/ void ConditionalOff(int x1,int y1,int x2,int y2);
/*?*/ void SetDoubleSpeedTreshould(unsigned imppersec);
/*?*/ void far* SwapInterrruptSubrutines(void far* address,MouseMask Mask,MouseMask& OldMask);
/*?*/ void AllocStateBufer();
/*?*/ void SaveMouseDriverState();
/*?*/ void RestoreMouseDriverState();
/*?*/ int SetAlternateInterruptSubrutine(void far* address,MouseMask Mask);
/*?*/ void far* GetAlternateInterruptSubrutine(MouseMask& Mask);
/*?*/ void SetMouseSensitivity(int sensX,int sensY,unsigned tres);
/*?*/ void GetMouseSensitivity(int& sensX,int& sensY,unsigned& tres);
/*?*/ void SetMouseInterruptRate(unsigned intpersec);
/*?*/ void SetCRTPageNumber(unsigned page);
/*?*/ unsigned GetCRTPageNumber();
/*?*/ void far* DisableDriver();
/*?*/ void EnableDriver();
/*?*/ int SoftwareReset();
};


/*
class pipek : MouseInterface
{
int x,y,xd,yd;
int color,ramka;
int keychar,cliksens;
char* text;
public:
void clear();
pipek();
int DoIt();
void InstallText(const char* t,int lkey,int lsens);
void set(int lx,int ly,int lxd, int lyd, int lcol , int lram);
void put();
void invers();
};
*/