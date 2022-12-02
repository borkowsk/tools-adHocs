unit RtfForm;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ComCtrls;

type
  TMainForm = class(TForm)
    GoButton: TButton;
    InstructionRichEdit: TRichEdit;
    procedure GoButtonClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormResize(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

const DefIniName='RtfView.ini';
var
  MainForm:TMainForm;
  IniFileName:string;
  RtfFileName:string='Instrukcja.rtf';
  RtfHigh:integer=0; //Jak 0 to domy�lne warto�ci ustawione w formularzu
  RtfWidth:integer=0;//Jak 0 to domy�lne warto�ci ustawione w formularzu
  GoButtonCaption:string='';//Jak pusty to domy�lne warto�ci ustawione w formularzu

implementation

{$R *.dfm}

procedure TMainForm.FormCreate(Sender: TObject);
var inifile:textFile;
    intpom:integer;
    strpom:string;
    blad:boolean;
begin
IniFileName:=ParamStr(1);
if IniFileName='' then
      IniFileName:=DefIniName;

if FileExists(IniFileName) then
begin
  AssignFile(inifile,IniFileName);
  Reset(inifile);

  strpom:='';
  Readln(inifile,strpom);
  if strpom<>'' then
      RtfFileName:=strpom;

  intpom:=-1;
  Readln(inifile,intpom);
  if intpom > 0 then
    RtfWidth:=intpom;

  intpom:=-1;
  Readln(inifile,intpom);
  if intpom > 0 then
    RtfHigh:=intpom;

  strpom:='';
  Readln(inifile,strpom);
  if strpom<>'' then
      GoButtonCaption:=strpom;

  CloseFile(inifile);
end
else
begin
 Application.MessageBox('Nie odnaleziono pliku konfiguracyjnego',PChar(IniFileName));
 Application.Terminate;
end;

//Pr�ba wy�wietlenia instrukcji
blad:=false;
try
if FileExists(RtfFileName) then
  self.InstructionRichEdit.Lines.LoadFromFile(RtfFileName)
  else blad:=true; //Tu si� obejdzie bez Exception, ale te� si� okno nie wy�wietli
except
  on E: Exception do  //Ignoruje te� inne b��dy ale o nich iunformuje
      blad:=true;
end;

if blad then
begin
  Application.MessageBox('Nie mo�na otworzy� pliku z tekstem',PChar(RtfFileName));
  Application.Terminate;
end;

//Parametry formularza z pliku ini o ile s�
if RtfHigh>=100 then
  self.ClientHeight:=RtfHigh;
if RtfWidth>=100 then
  self.ClientWidth:=RtfWidth;
if GoButtonCaption<>'' then
  self.GoButton.Caption:=GoButtonCaption;

end;

procedure TMainForm.FormResize(Sender: TObject);
begin
 InstructionRichEdit.Repaint;
end;

procedure TMainForm.GoButtonClick(Sender: TObject);
var inifile:textFile;
begin
//Zapami�tywanie ostatnich warto�ci formularza
AssignFile(inifile,IniFileName);
Rewrite(inifile);
writeln(inifile,RtfFileName);
writeln(inifile,self.ClientWidth);
writeln(inifile,self.ClientHeight);
writeln(inifile,self.GoButton.Caption);
closeFile(inifile);
self.Close;
end;

end.
