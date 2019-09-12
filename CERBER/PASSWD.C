
/* Program zmieniajacy haslo uzytkownika */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include <dir.h>
//#include <conio.h>

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

unsigned int user_id=0;
void setonerror()
{
SetUser(user_id);
}

#pragma exit setonerror

int GetUser_Id(void)
{
_AX=0xFF00;
geninterrupt(0x21);
if(_AX==0x0F0F) return _CX;
	else    return -1;
}


char users_tab[256][8];/* Tablica identyfikatorow uzytkownikow */
char line[255];
char user[9],pass[9]="*";
int id;

void init_users_tab(int U)/*Funkcja wczytujaca plik passwd do uzytkownika U */
{
char *s;char drive[MAXDRIVE];char dir[MAXDIR];char file[MAXFILE];char ext[MAXEXT];
char b[MAXPATH];
FILE* passfile;
s=getenv("COMSPEC"); /* get the comspec environment parameter */
fnsplit(s,drive,dir,file,ext);
sprintf(b,"%s%sPASSWD",drive,dir);/* passwd file must by in thy same directory */
if((passfile=fopen(b,"r"))==NULL)
		fprintf(stderr,"\npasswd:Cant open file %s !\n",b);
		else
		{
		while(!feof(passfile))
			{
			fgets(line,128,passfile);
			sscanf(line,"%8s %8s %d ",user,pass,&id);
			strncpy(users_tab[id],user,8);
			if(id==U) break;
			}
		fclose(passfile);
		if(id!=U)
			{fprintf(stderr,"Invalid user id %d. Last valid is %d !\n",U,id);exit(1);}
		}
}

void newpsswd(unsigned user_id,char* npass)
{
char *s;char drive[MAXDRIVE];char dir[MAXDIR];char file[MAXFILE];char ext[MAXEXT];
char b[MAXPATH],c[MAXPATH];
FILE* passfile,*newfile;
printf("OK - WAIT !\n");
s=getenv("COMSPEC"); /* get the comspec environment parameter */
fnsplit(s,drive,dir,file,ext);
sprintf(b,"%s%sPASSWD.TMP",drive,dir);/* Na wsiakij sluczaj ! */
if(_chmod(b,1,0)!=-1) 		      /* Change atributes */
	{perror("Chmod PASSWD.TMP file");}
sprintf(b,"%s%sPASSWD",drive,dir);/* passwd file must by in thy same directory */
if((passfile=fopen(b,"rt"))==NULL)
		{
		fprintf(stderr,"\npasswd:No passwd file !\n");
		/* New file creation */
		sprintf(b,"%s%sPASSWD.TMP",drive,dir);/* passwd file must by in thy same directory */
		if((newfile=fopen(b,"wt"))==NULL)
			{fprintf(stderr,"\npasswd:Cant write !\n");
			 perror(""); exit(103);}
		fprintf(newfile,"%8s %8s %d\n","nobody","*",0);
		if(strncmp(npass,"\0",8)==0)
					*npass='*';
		fprintf(newfile,"%8s %8s %d\n","root",npass,1);
		if( user_id!=1 && user_id!=0 )
			{
			fprintf(newfile,"%8s %8s %d\n",users_tab[user_id],npass,user_id);
			}
		fclose(newfile);
		}
		else
		{
		/* New file creation */
		sprintf(b,"%s%sPASSWD.TMP",drive,dir);/* passwd file must by in thy same directory */
		if((newfile=fopen(b,"w"))==NULL)
			{fprintf(stderr,"\npasswd:Cant write !\n");
			 perror(""); exit(103);}
		while(!feof(passfile))
			{
			char lpass[9];
			if(fgets(line,128,passfile)==NULL) break;
			sscanf(line,"%8s %8s %d ",user,lpass,&id);
			if(id!=user_id)
				fprintf(newfile,"%8s %8s %d\n",user,lpass,id);
				else
				fprintf(newfile,"%8s %8s %d\n",user,npass,user_id);
			}
		fclose(passfile);
		fclose(newfile);
		}
errno=0;*c='\0';*b='\0';
if(SetUser(1))
	{perror("Fatal error S1");exit(1);}

sprintf(c,"%s%sPASSWD.OLD",drive,dir);
if(_chmod(c,1,0)==-1)  /* Change atributes */
	{perror("Cant chmod PASSWD.OLD file");errno=0;}

sprintf(b,"%s%sPASSWD",drive,dir);
if(_chmod(b,1,0)==-1)  /* Change atributes */
	{perror("Cant chmod PASSWD file");errno=0;}

sprintf(b,"%s%sPASSWD.TMP",drive,dir);
if(_chmod(b,1,0)==-1)  /* Change atributes */
	{perror("Cant chmod PASSWD.TMP file");errno=0;}

sprintf(c,"%s%sPASSWD.OLD",drive,dir);
if(unlink(c)==-1)	/* Destroy old old copy */
	{perror("Cant unlink PASSWD.OLD file");errno=0;}


sprintf(b,"%s%sPASSWD",drive,dir);
sprintf(c,"%s%sPASSWD.OLD",drive,dir);
if(rename(b,c)==-1)	/* Save old passwd */
	{fprintf(stderr,"Cant rename %s to %s",b,c);perror("");errno=0;}

sprintf(b,"%s%sPASSWD.TMP",drive,dir);
sprintf(c,"%s%sPASSWD",drive,dir);
if(rename(b,c)==-1) /* Set new passwd file */
	 {fprintf(stderr,"Cant rename %s to %s",b,c);perror("");errno=0;}
if(unlink(b)==0)	/* Destroy TMP copy if exist */
	{perror("Succesfully unlink PASSWD.TMP file");errno=0;}
{int i;/* Prowizorka - dezaktualizacja buforow plikow */
sprintf(b,"\PASSWD.TMP",drive,dir);/* passwd file must by in the same directory */
if((newfile=fopen(b,"w"))==NULL)
	{fprintf(stderr,"\npasswd:Cant write !\n");
	 perror("");}
for(i=0;i<(30*512);i++)
	fprintf(newfile,"&");
fclose(newfile);
}
if(unlink(b)==0)	/* Destroy TMP copy if exist */
	{perror("Succesfully unlink \PASSWD.TMP file");errno=0;}
if(_chmod(c,1,FA_RDONLY)==-1)  /* Change atributes */
	{perror("Cant chmod PASSWD file");errno=0;}

if(SetUser(user_id))
	{fprintf(stderr,"Security program mismash !");exit(2);}
if(errno!=0)
	perror("system");
	else
	perror("DONE");
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
user_id=GetUser_Id();/* Aktualnie pracujacy uzytkownik */
if(user_id==0xFFFF) { printf("\nCERBER is not active !\n");return 1;}
if(user_id==0)	{printf("Unable for user NOBODY !\n");return 1;}
init_users_tab(user_id);
printf("\n\nChange password for user %d - %s .\nOld password:",id,user);
Getpass(b);
if(EQpassword(crypto(b),pass))/* POROWNANIE PAMIENTANEGO HASLA Z WPROWADZONYM */
	{printf("Invalid password !");exit(1);}
	else
	{
	printf("New password (maximum 8 characters):");
	Getpass(b);
	printf("Retype new password:");
	Getpass(pass);
	if(EQpassword(b,pass))
		{
		printf("Sorry ! Not equal !\n");
		exit(1);
		}
		else
		{
		strcpy(pass,crypto(pass));/* Zakodowanie hasla */
		if( strncmp(users_tab[user_id],"\0",8)==0)
			{
			printf("Type carefully your user id: \n");
			fgets(users_tab[user_id],8,stdin);
			}
		newpsswd(user_id,pass);
		}
	}
return 0;
}