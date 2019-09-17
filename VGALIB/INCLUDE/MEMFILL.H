/* Funkcje wypelniania pamieci, dla kompilatora BOrland 2.0 */
static
void near memfill(unsigned char far* cel,int c,unsigned int n)
{
asm{
push ES
cld
mov CX,n
mov AX,c
les DI,cel
rep stosb
pop  ES
}
}


#define MEMFILL(/*unsigned char far* */_cel_,/*int*/_c_,/*unsigned int*/_n_) \
{unsigned char far* cel=(unsigned char far*)_cel_;\
unsigned int n=(unsigned int )_n_;\
asm{ 		\
push CX	;	\
push ES ;	\
push DI ;	\
cld     ;	\
mov CX,n;	\
mov AX,_c_;	\
les DI,cel;	\
rep stosb; 	\
pop  DI  ;	\
pop  ES  ;	\
pop  CX  ;      \
}}
