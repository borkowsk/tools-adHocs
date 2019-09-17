/*
Bibliotek Workspace User Interface - rozne typy fontow
*/
class font8bit : font
{
char*  mattrix; /* Pointer to table of matrix */
int    hight;   /* Byte per matrix */
virtual pixsize_t TextWidth(const char* t);
virtual pixsize_t TextHight(const char* t);
virtual int OutTextXY(picture& p,pixco_t x,pixco_t y,const char* t);
public:
font8bit(char* m,int h)
	{
	mattrix=m;
	hight=h;
	}
};