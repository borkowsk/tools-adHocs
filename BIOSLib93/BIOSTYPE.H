/*
POMOCNICZE FUNKCJE OPARTE NA BIOSIE  -  TYPY I STALE
*/
#define TEXTMODE   (*((unsigned char far*)0x00400049))
#define PAGEOFFSET (*((unsigned int  far*)0x0040004e))
#define RIGHT  0x01
#define LEFT   0x02
#define CTRL   0x04
#define ALT    0x08
#define NUM    0x10
#define SCROLL 0x20
#define CAPS   0x40
#define INS    0x80

//   DEfinicje typow
//---------------------

typedef unsigned char byte;
typedef unsigned int word;
typedef struct {
		unsigned ch:8;
		unsigned color:3;
		unsigned light:1;
		unsigned back:3;
		unsigned blink:1;
		 } textpoint;

#define textwiersz textline
typedef textpoint textline[LineLong];

/* Funkcje */
void gotoxy(unsigned int x,unsigned int y);
void dosbeep();
void portbeep(unsigned int count);
int  bioskey(unsigned int cmd);
