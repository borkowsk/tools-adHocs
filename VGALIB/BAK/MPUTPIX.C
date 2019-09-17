void   far NewSetPixel58(long int x , long int y ,unsigned int color)
/* Procedura rysowania pixela na EGA i VGA wg KONSYLIUM -ENTER VIII 1992 */
{
char lay;
int row=y;
int col=x;
char bitMask=(char)(0x80 > (col & 7)); // >> ?
unsigned
char far* baddr= (unsigned char far*) (0xA0000000+(col>>3)+(row*peek(0x40,0x4a)));

outp(0x3CE, 8);
outp(0x3CF, bitMask);
lay=*(baddr);
*(baddr)=0;
outp(0x3C4, 2);
outp(0x3C5, color);
*(baddr)=0xFF;
/* RESTORE */
outp(0x3C4, 2);
outp(0x3C5, 0xF);
outp(0x3CE, 8);
outp(0x3CF, 0xFF);
}

char* unpack(unsigned char* bool,unsigned char pom)
{
bool[0]=(pom &  0x01);
bool[1]=(pom &  0x02)>>1;
bool[2]=(pom &  0x04)>>2;
bool[3]=(pom &  0x08)>>3;
bool[4]=(pom &  0x10)>>4;
bool[5]=(pom &  0x20)>>5;
bool[6]=(pom &  0x40)>>6;
bool[7]=(pom &  0x80)>>7;
return bool;
}