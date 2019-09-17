#include <string.h>
#include <dos.h>
typedef unsigned char byte;
typedef unsigned int word;
typedef struct {
		byte ch;
		byte at;
		} textpoint;
typedef byte wiersz160[160];
typedef textpoint textwiersz[80];

textwiersz far *ekranTEXT=NULL ;
byte      far *ekranHERC=NULL;
void      far *ekranBYTE=NULL;

main()
 {

/*ekranBYTE=MK_FP( 0xB000 ,0x0000 ); ekran tekstowy herculesa*/
ekranBYTE=MK_FP(0xB800 ,0x0000);/* ekran tekstowy VGA */
{
byte far *pom=(byte far*)ekranBYTE;
unsigned int i;
for(i=0;i<4000;i++) if(i%2) *pom++=4; 
		       else *pom++='-';
}

 ekranTEXT=(textwiersz far *)ekranBYTE;
 ekranHERC=(byte far*)ekranBYTE;

 ekranTEXT[0][12].ch='#';
 ekranTEXT[1][12].ch='H';
{
byte i;  
for(i=0;i<=25;i++)
	ekranTEXT[i][i].ch='/';
}
 return 1;
 }