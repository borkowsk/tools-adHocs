/* Program podaje identyfikator aktualnego uzytkownika */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <dir.h>

int GetUser_Id(void)
{
_AX=0xFF00;
geninterrupt(0x21);
if(_AX==0x0F0F) return _CX;
	else    return -1;
}

char users_tab[256][8];/* Tablica identyfikatorow uzytkownikow */
void init_users_tab(void)/*Funkcja wczytujaca plik passwd */
{
char *s;char drive[MAXDRIVE];char dir[MAXDIR];char file[MAXFILE];char ext[MAXEXT];
char b[MAXPATH];
FILE* passfile;
s=getenv("COMSPEC"); /* get the comspec environment parameter */
fnsplit(s,drive,dir,file,ext);
sprintf(b,"%s%sPASSWD",drive,dir);/* passwd file must by in thy same directory */
if((passfile=fopen(b,"r"))==NULL)
		fprintf(stderr,"GETUSER:Cant open file %s !\n",b);
		else
		{
		char line[128];
		char user[9],pass[9];
		unsigned id;
		while(!feof(passfile))
			{
			fgets(line,128,passfile);
			sscanf(line,"%8s %8s %u ",user,pass,&id);
			strncpy(users_tab[id],user,8);
			}
		fclose(passfile);
		}
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
main()
{
char b[12];
int pom=GetUser_Id();
init_users_tab();
if(pom==-1) printf("CERBER is not active !\n");
   else     printf("User ID is %d - %s \n",pom,s_id(b,pom));
return pom;
}