/* Program zmieniajacy identyfikator uzytkownika */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <dos.h>
#include <dir.h>

int SetUser(unsigned int NewId)
{
_BX=NewId;
_CX=HiPass;
_DX=LoPass;
_AX=0xFF01;
geninterrupt(0x21);
asm jc blad;
return 0;
blad:
return -1;
}

int GetUser_Id(void)
{
_AX=0xFF00;
geninterrupt(0x21);
if(_AX==0x0F0F) return _CX;
	else    return -1;
}



char users_tab[256][8];/* Tablica identyfikatorow uzytkownikow */
char passw_tab[256][8];/* Tablica hasel uzytkownikow */
char line[255];
char user[9],pass[9]="*";
int id;

void init_users_tab(int U)/*Funkcja wczytujaca plik passwd do uzytkownika U */
{
char *s;char drive[MAXDRIVE];char dir[MAXDIR];char file[MAXFILE];char ext[MAXEXT];
char b[MAXPATH];
FILE* passfile;
char User[9],Pass[9];
int Id;
s=getenv("COMSPEC"); /* get the comspec environment parameter */
fnsplit(s,drive,dir,file,ext);
sprintf(b,"%s%sPASSWD",drive,dir);/* passwd file must by in thy same directory */
if((passfile=fopen(b,"r"))==NULL)
		fprintf(stderr,"passwd:Cant open file %s !\n",b);
		else
		{
		while(!feof(passfile))
			{
			fgets(line,128,passfile);
			sscanf(line,"%8s %8s %d ",User,Pass,&Id);
			strncpy(users_tab[Id],User,8);
			strncpy(passw_tab[Id],Pass,8);
			if(Id==U)
				{
				strcpy(user,User);
				strcpy(pass,Pass);
				id=Id;
				}
			}
		fclose(passfile);
		}
}

char* Getpass(char* buf)
{
int x=wherex();
int y=wherey();
int c,l=0;
while((c=getch())!=13 && l<8)
	{
	buf[l]=c;
	l++;
	gotoxy(x+l,y);
	}
buf[l]='\0';
putchar('\n');
return buf;
}

int EQpassword(char* p1,char* p2)
{
return strcmp(p1,p2);
}

char* crypto(char* wh)
{
static char ou[20];
int i,a=strlen(wh);
if(a==0)
      {
      ou[0]='*';
      ou[1]='\0';
      return ou;
      }
for(i=0;i<a;i++)
	{
	ou[i]=wh[i]+a+i;
	if(ou[i]==' ') abort();
	}
return ou;
}

char* s_id(char* s,unsigned char id)
{
if(id==0)
	sprintf(s," nobody ");
	else
	if(id==1)
	   sprintf(s,"operator");
	   else
	   if(users_tab[id][0]!='\0')
		sprintf(s,"%-8.8s",users_tab[id]);
		else
		sprintf(s,"  %03u  ",id);
return s;
}

unsigned char str2user(const char* s)
{
unsigned char c;
char *end;
if(isdigit(s[0]))
	c=strtol(s, &end , 0 );
	else
	for(c=255;c>0;c--)
	 if(strncmp(users_tab[c],s,8)==0) break;
return c;
}

main()
{
char b[12];
unsigned int user_id=GetUser_Id();/* Aktualnie pracujacy uzytkownik */
if(user_id==0xFFFF) { printf("CERBER is not active !\n");goto ERROR;}
if(_argv[1]==NULL)
	{
	printf("USAGE:>  su user\n");
	return 1;
	}
init_users_tab(255);
user_id=str2user(_argv[1]);
printf("Change user ID to %d - %s \n",user_id,s_id(b,user_id));
if(user_id!=0)
{
/* Password */
printf("Password:");
Getpass(b);
if( EQpassword( crypto(b) ,passw_tab[user_id])==0 )
	{
	if(SetUser(user_id))
			goto ERROR;
	user_id=GetUser_Id();
	printf("OK - Set to %d - %s \n",user_id,s_id(b,user_id));
	return 0;
	}
printf("Invalid password !");
}
else SetUser(0);
return 1;
ERROR:
printf("Security program mismash !\n");
return 2;
}

