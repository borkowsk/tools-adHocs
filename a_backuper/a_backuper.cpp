// a_backuper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "resource.h"
#include <Windows.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h> 
#include <direct.h>
#include <errno.h> 
#include <iostream.h>
#include <strstrea.h>
#include <iomanip.h>

#include "iter_dir.hpp"
#include "wb_ptr.hpp"


//ostream& operator << (ostream& o,time_t t)//UWAGA NIEBEZPIECZNY POMYSL - KAZDY long MOZE BYC TRAKTOWANY JAK DATA!!!
ostream& out_data(ostream& o,time_t t)
{
    struct tm Ti=*localtime(&t);
    o<<setfill('0')
     <<Ti.tm_year+1900<<'/'<<setw(2)<<Ti.tm_mon+1<<'/'<<setw(2)<<Ti.tm_mday
     <<setfill(' ');
    return o;
}

//INFO ABOUT THE CURRENT SOURCE VOLUME
char VolumeNameBuffer[1024]="";  
DWORD VolumeSerialNumber=0;
DWORD MaximumComponentLength=0;
DWORD FileSystemFlags=0;
char FileSystemNameBuffer[1024]="";
time_t MinTime=time(NULL);//Now!
time_t MaxTime=0;

char* MakeTargetName()
{
    ostrstream Out;
    if(MinTime==-1 || MinTime==0)
        MinTime=MaxTime;
    else
    if(MaxTime==-1 || MaxTime==0)
        MaxTime=MinTime;

    struct tm Min;
    struct tm Max;
    Min=*localtime(&MinTime);
    Max=*localtime(&MaxTime);
    Out<<setfill('0');
    if(MinTime!=MaxTime)
    {
       Out<<Min.tm_year+1900<<'_'<<setw(2)<<Min.tm_mon+1<<setw(2)<<Min.tm_mday<<'-';
    }
    Out<<Max.tm_year+1900<<'_'<<setw(2)<<Max.tm_mon+1<<setw(2)<<Max.tm_mday
       <<(VolumeNameBuffer[0]?"_":"")
       <<(VolumeNameBuffer[0]?VolumeNameBuffer:"")<<'\0'<<'\0'<<'\0';
    return Out.str();
}

bool DetermineTimePeriod(const char* VolPath)
{
    directory_iterator Dir(VolPath,"*");
    if(!Dir.IsOK())
        return false;

    for(int n=0;Dir!=0;Dir++)
    {        
        //cout<<Dir.get_name()<<"-->"<<*Dir<<endl<<flush;    
        if((*Dir)[0]=='.')
            continue; 
        
        wb_pchar pom;
        pom.take(Dir.get_item_real_name());
        
        cout<<pom.get()<<flush;  
        
        struct stat buf;
        bool result=Dir.get_stat(buf);// Get data associated with path
        if( !result )
        {
            //perror( "Error reading info about it!" );
            return false;
        }
        else
        {
            //SPRAWDZANIE CZASU UTWORZENIA I MODYFIKACJI DLA KA¯DEGO PLIKU
            cout<<' ';
            if(buf.st_ctime!=-1)
                out_data(cout,buf.st_ctime)<<' ';
            if(buf.st_mtime!=-1)
                out_data(cout,buf.st_mtime)<<' ';
            cout<<flush;
            
            time_t  better=(buf.st_ctime!=-1?buf.st_ctime:0);
            if(better<buf.st_mtime) //ctime bywa 0 albo -1 
                better=buf.st_mtime;

            if(better!=0 && better!=-1)
            {
                if(better>MaxTime)
                    MaxTime=better;
                if(better<MinTime)
                    MinTime=better;
            }
        }

        cout<<endl;

        if(Dir.is_subdir())
        {
            
            bool ret= DetermineTimePeriod(pom.get());
            if(!ret)
                return false;
        }
    }
    
    cout<<endl;
    return true;
}

BOOL ReadInfoAboutVolume(char* VolPath)
{
    BOOL ret=GetVolumeInformation(
        VolPath,                                          //LPCTSTR ,  address of root directory of the file system        
        VolumeNameBuffer,                                 //LPTSTR ,   address of name of the volume
        1024,     //DWORD ,    length of lpVolumeNameBuffer        
        &VolumeSerialNumber,                            //LPDWORD ,  address of volume serial number
        &MaximumComponentLength,                       //LPDWORD address of system's maximum filename length                                
        &FileSystemFlags,                              //LPDWORD address of file system flags        
        FileSystemNameBuffer,                         //LPTSTR  address of name of file system
        1024      // length of lpFileSystemNameBuffer
        );
    
    if(ret)
    {
        cout<<"VOLUME: '"<<VolPath<<"' SERIAL# '"<<VolumeSerialNumber<<"'"<<endl;
        cout<<"LABELED: '"<<VolumeNameBuffer<<"' FILE SYSTEM: '"<<FileSystemNameBuffer<<"'"<<endl;
    }
    else
    {
        DWORD erorr=GetLastError();
    }

    return ret;
}

extern "C"
BOOL CALLBACK DialogProc(HWND  hDlg,UINT   message,WPARAM   wParam,LPARAM   lParam) 
{ 
    switch(message) 
    { 
    case WM_INITDIALOG: 
        {
            return TRUE; 
        }
    case WM_COMMAND: 
        {
            unsigned int Lo=LOWORD(wParam);
            unsigned int Hi=HIWORD(wParam);
            switch(Lo) 
            { 
            case 2://End
            case IDOK:
                DestroyWindow(hDlg);
                return TRUE; 
            }
            return FALSE;
        }
    default:
        {
            return FALSE;
        }
    }
}

bool CopyDirTree1(const char* From,const char* To)
{
    LPTSTR DiaId=MAKEINTRESOURCE(ID_DIALOG1);       //DILOG ID FROM RESOURCES
	HINSTANCE hMainInstance=GetModuleHandle(NULL);	//APLICATION INSTANCE	
	HWND Dialog=CreateDialog(
				hMainInstance,
				DiaId,
				0,//OWNER WINDOW
				DialogProc);
	ShowWindow(Dialog,SW_SHOW);     //OK - IT WORK
    
    SHFILEOPSTRUCT op;
    op.hwnd=Dialog;
    op.wFunc=FO_COPY;
    op.pFrom=From;
    op.pTo=To;
    op.fFlags=FOF_NOCONFIRMMKDIR;// | FOF_RENAMEONCOLLISION | FOF_SIMPLEPROGRESS;
    op.fAnyOperationsAborted=0;
    op.hNameMappings=0;
    op.lpszProgressTitle="A_BACKUP IN PROGRESS";

    int ret=SHFileOperation(&op);   //!!! CAN'T READ SOURCE FILE OR DISK (a:\ or a:\* WAS TESTED)

    DestroyWindow(Dialog);      //OK - IT WORK

    if(ret!=0)
    {
        DWORD erorr=GetLastError();
        return false;
    }
    else
    {
        return true;
    }
}

bool CopyDirTree2(const char* From,const char* To)
{
    //SHELLEXECUTEINFO op;
    //ShellExecuteEx()
    wb_pchar op(2048);
    op.prn("cmd /C xcopy %s \"%s\\\"  /I /E /H",From,To);
    cerr<<op.get()<<endl;
    int ret=system(op.get());
    return ret==0 && errno!=ENOENT;
}

bool CopyDirTree3(const char* From,const char* To)
{
    wb_pchar op(2048);
    //op.prn("/C xcopy %s %s\\  /I /E",From,To); //for cmd.exe  
    //HINSTANCE ret=ShellExecute(0,"OPEN","cmd",op.get(),NULL,SW_SHOWNORMAL);
    
    op.prn("%s %s\\  /I /E",From,To);           //for xcopy.exe
    //cerr<<op.get()<<endl;
    HINSTANCE ret=ShellExecute(0,"OPEN","xcopy",op.get(),NULL,SW_SHOWNORMAL);

    if(int(ret)>32)
    {
        return true;
    }
    else
    {
        DWORD erorr=GetLastError();
        return false;
    }

}

int main(int argc, char* argv[])
{
    int i;//Zmienna sterujaca petli po parametrach

    cout<<"\"A: BACKUPER\" by Wojciech Borkowski (www.iss.uw.edu.pl/~borkowsk/)\n"
        <<"EN:The smart backuping of small volumes into the directory of big one\n"
        <<"PL:Sprytne kopiowanie z dysku przenoœnego do katalogu dysku lokalnego\n  ";
        
    //Sprawdzenie parametrów wywo³ania 
    for(i=0;i<argc;i++)
        cout<<argv[i]<<' ';
    cout<<endl<<flush;
    
    //Czytanie etykiety dysku
    for(i=1;i<argc;i++)
    {
        UINT previous=SetErrorMode(0 /* albo SEM_FAILCRITICALERRORS*/);// bit flags specifying the process error mode
        if(ReadInfoAboutVolume(argv[i]))
        {
            if(DetermineTimePeriod(argv[i]))
            {
                wb_pchar source_name(100);
                source_name.prn("%s%s\0\0",argv[i],"*");//*.* ???
                //source_name.prn("%s","c:\\tmp\\");
                cout<<source_name.get()<<"-->";
                
                wb_pchar target_name;
                target_name.take(MakeTargetName());
                cout<<target_name.get()<<endl;

                if(mkdir(target_name.get())==-1)
                {
                    perror(target_name.get());
                    MessageBox(0,"Zadanie przerwane. Nie mo¿na utworzyæ katalogu docelowego","\"A: BACKUPER\"",MB_OK | MB_ICONHAND );
                    break;
                }
                
                if(!CopyDirTree2(source_name.get(),target_name.get()))
                {
                    MessageBox(0,"Zadanie przerwane. B³¹d w trakcie kopiowania","\"A: BACKUPER\"",MB_OK | MB_ICONHAND );
                    break;
                }                
            }
            else
            {
                MessageBox(0,"Zadanie przerwane na skutek b³êdu odczytu","\"A: BACKUPER\"",MB_OK | MB_ICONHAND );
            }

            cout<<endl<<flush;
        }
        SetErrorMode(previous);
    }
 
    MessageBeep( MB_ICONASTERISK );
    MessageBox(0,"Zadanie zakoñczone","\"A: BACKUPER\"",MB_OK | MB_ICONASTERISK);
	return 0;
}
