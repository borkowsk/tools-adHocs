unit unitypes;
interface
uses multitype;
type pstring=^string;
     pboolean=^boolean;
     pbyte=^byte;
     pinteger=^integer;
     pword=^word;
     plongint=^longint;
     pdouble=^double;
     psingle=^single;

const falseString='No/False/Nie';
      trueString='Yes/True/Tak';
type enum_name_str=string[32];
     ttab=array[0..1023] of enum_name_str;

type Ustring=object(multiroot)
     constructor init(v:pstring;l:byte);
     destructor done;                      virtual;
     function FromString(s:string):boolean;virtual;
     function Fromdouble(r:double):boolean;    virtual;
     function FromInt(i:longint):boolean;  virtual;
     function AsString:string;             virtual;
     function AsInteger:longint;           virtual;
     function Asdouble:double;                 virtual;
     function AsStringLen(l:word):string;  virtual;
     public
     len:byte;
     val:pstring  ;
     end;

type Uboolean=object(multiroot)
     constructor init(v:pboolean );
     destructor done;                      virtual;
     function FromString(s:string):boolean;virtual;
     function Fromdouble(r:double):boolean;    virtual;
     function FromInt(i:longint):boolean;  virtual;
     function AsString:string;             virtual;
     function AsInteger:longint;           virtual;
     function Asdouble:double;                 virtual;
     function AsStringLen(l:word):string;  virtual;
     public
     val:pboolean;
     end;

type Uinteger=object(multiroot)
     constructor init(v:pinteger );
     destructor done;                      virtual;
     function FromString(s:string):boolean;virtual;
     function Fromdouble(r:double):boolean;    virtual;
     function FromInt(i:longint):boolean;  virtual;
     function AsString:string;             virtual;
     function AsInteger:longint;           virtual;
     function Asdouble:double;                 virtual;
     function AsStringLen(l:word):string;  virtual;
     public
     val:pinteger;
     end;

type Uword=object(multiroot)
     constructor init(v:pword );
     destructor done;                      virtual;
     function FromString(s:string):boolean;virtual;
     function Fromdouble(r:double):boolean;    virtual;
     function FromInt(i:longint):boolean;  virtual;
     function AsString:string;             virtual;
     function AsInteger:longint;           virtual;
     function Asdouble:double;                 virtual;
     function AsStringLen(l:word):string;  virtual;
     public
     val:pword;
     end;

type Ubyte=object(multiroot)
     constructor init(v:pbyte );
     destructor done;                      virtual;
     function FromString(s:string):boolean;virtual;
     function Fromdouble(r:double):boolean;    virtual;
     function FromInt(i:longint):boolean;  virtual;
     function AsString:string;             virtual;
     function AsInteger:longint;           virtual;
     function Asdouble:double;                 virtual;
     function AsStringLen(l:word):string;  virtual;
     public
     val:pbyte;
     end;

type Uenum=object(uword)
     constructor init(v:pword;nn:integer;var nazwy);
     destructor done;                      virtual;
     function FromString(s:string):boolean;virtual;
     function AsString:string;             virtual;
     function AsStringLen(l:word):string;  virtual;
     public
     n:integer;
     tab:pointer;
     end;

type Ulongint=object(multiroot)
     constructor init(v:plongint );
     destructor done;                      virtual;
     function FromString(s:string):boolean;virtual;
     function Fromdouble(r:double):boolean;    virtual;
     function FromInt(i:longint):boolean;  virtual;
     function AsString:string;             virtual;
     function AsInteger:longint;           virtual;
     function Asdouble:double;                 virtual;
     function AsStringLen(l:word):string;  virtual;
     public
     val:plongint ;
     end;

type Udouble=object(multiroot)
     constructor init(v:pdouble );
     destructor done;                      virtual;
     function FromString(s:string):boolean;virtual;
     function Fromdouble(r:double):boolean;    virtual;
     function FromInt(i:longint):boolean;  virtual;
     function AsString:string;             virtual;
     function AsInteger:longint;           virtual;
     function Asdouble:double;                 virtual;
     function AsStringLen(l:word):string;  virtual;
     public
     val:pdouble;
     end;

type Usingle=object(multiroot)
     constructor init(v:psingle );
     destructor done;                      virtual;
     function FromString(s:string):boolean;virtual;
     function FromDouble(r:double):boolean;    virtual;
     function FromInt(i:longint):boolean;  virtual;
     function AsString:string;             virtual;
     function AsInteger:longint;           virtual;
     function AsDouble:double;                 virtual;
     function AsStringLen(l:word):string;  virtual;
     public
     val:psingle;
     end;


type puni=^multiroot;
     uni=multiroot;
     pustring=^Ustring;
     puboolean=^Uboolean;
     puenum=^Uenum;
     puword=^Uword;
     puinteger=^Uinteger;
     pulongint=^Ulongint;
     pudouble=^Udouble;

const MAXdouble=1.7e308;
      MAXsingle=3.4e38;

implementation

constructor Ustring.init(v:pstring;l:byte);
begin
val:=v;
len:=l;
end;
destructor Ustring.done;
begin
end;

function Ustring.Fromdouble(r:double):boolean;
var s:string;
begin
str(r:0:len,s);
val^:=copy(s,1,len);
end;

function Ustring.FromInt(i:longint):boolean;
var s:string;
begin
str(i:len,s);
val^:=copy(s,1,len);
end;

function Ustring.AsInteger:longint;
var i:longint;
    e:integer;
begin
i:=0;
system.val(val^,i,e);{ignore errors!}
AsInteger:=i;
end;

function Ustring.Asdouble:double;
var r:double;
    e:integer;
begin
r:=0;
system.val(val^,r,e);{ignore errors!}
Asdouble:=r;
end;

constructor Uboolean.init(v:pboolean);
begin
val:=v;
end;

destructor Uboolean.done;
begin
end;

function Uboolean.Fromdouble(r:double):boolean;
begin
if r=0 then val^:=false
       else val^:=true;
end;

function Uboolean.FromInt(i:longint):boolean;
begin
if i=0 then val^:=false
       else val^:=true;
end;

function Uboolean.AsInteger:longint;
begin
AsInteger:=byte(val^);
end;

function Uboolean.Asdouble:double;
begin
Asdouble:=byte(val^);
end;

constructor Uinteger.init(v:pinteger);
begin
val:=v;
end;
destructor Uinteger.done;
begin
end;
function Uinteger.Fromdouble(r:double):boolean;
begin
val^:=round(r);
end;
function Uinteger.FromInt(i:longint):boolean;
begin
val^:=i;
end;
function Uinteger.AsInteger:longint;
begin
AsInteger:=val^;
end;
function Uinteger.Asdouble:double;
begin
Asdouble:=val^;
end;

constructor Uword.init(v:pword);
begin
val:=v;
end;
destructor Uword.done;
begin
end;
function Uword.Fromdouble(r:double):boolean;
begin
val^:=round(r);
end;
function Uword.FromInt(i:longint):boolean;
begin
val^:=i;
end;
function Uword.AsInteger:longint;
begin
AsInteger:=val^;
end;
function Uword.Asdouble:double;
begin
Asdouble:=val^;
end;

constructor Ubyte.init(v:pbyte);
begin
val:=v;
end;
destructor Ubyte.done;
begin
end;
function Ubyte.Fromdouble(r:double):boolean;
begin
val^:=round(r);
end;
function Ubyte.FromInt(i:longint):boolean;
begin
val^:=i;
end;
function Ubyte.AsInteger:longint;
begin
AsInteger:=val^;
end;
function Ubyte.Asdouble:double;
begin
Asdouble:=val^;
end;

constructor Uenum.init(v:pword;nn:integer;var nazwy);
begin
val:=v;
n:=nn;
tab:=@nazwy;
end;
destructor Uenum.done;
begin
n:=0;
tab:=nil;
end;

constructor Ulongint.init(v:plongint);
begin
val:=v;
end;
destructor Ulongint.done;
begin
end;
function Ulongint.Fromdouble(r:double):boolean;
begin
val^:=round(r);
end;

function Ulongint.FromInt(i:longint):boolean;
begin
val^:=i;
end;

function Ulongint.AsInteger:longint;
begin
AsInteger:=val^;
end;

function Ulongint.Asdouble:double;
begin
Asdouble:=val^;
end;

constructor Udouble.init(v:pdouble);
begin
val:=v;
end;
destructor Udouble.done;
begin
end;

function Udouble.Fromdouble(r:double):boolean;
begin
val^:=r;
end;
function Udouble.FromInt(i:longint):boolean;
begin
val^:=i;
end;
function Udouble.AsInteger:longint;
begin
AsInteger:=round(val^);
end;
function Udouble.Asdouble:double;
begin
Asdouble:=val^;
end;

constructor Usingle.init(v:psingle);
begin
val:=v;
end;
destructor Usingle.done;
begin
end;

function Usingle.Fromdouble(r:double):boolean;
begin
val^:=r;
end;
function Usingle.FromInt(i:longint):boolean;
begin
val^:=i;
end;
function Usingle.AsInteger:longint;
begin
AsInteger:=round(val^);
end;
function Usingle.Asdouble:double;
begin
Asdouble:=val^;
end;

function Ustring.FromString(s:string):boolean;
begin
val^:=copy(s,1,len);
FromString:=(length(s)<=len); {False gdy obcieto}
end;

function Uboolean.FromString(s:string):boolean;
var i:extended;
    e:integer;
begin
fromString:=true;
s[1]:=UpCase(s[1]);
if (s[1]='Y')or(s[1]='T') then begin val^:=true;exit; end;
if (s[1]='N')or(s[1]='F') then begin val^:=false; exit; end;
FromString:=false;
end;

function Uinteger.FromString(s:string):boolean;
var i:extended;
    e:integer;
begin
i:=0;
system.val(s,i,e);{ignore errors!}
if (e<>0)or(i<-MAXINT)or(i>MAXINT) then
   begin FromString:=False; exit end;
val^:=round(i);
FromString:=True;
end;

function Uword.FromString(s:string):boolean;
var i:extended;
    e:integer;
begin
i:=0;
system.val(s,i,e);{ignore errors!}
if (e<>0)or(i<0)or(i>$ffff) then
   begin FromString:=False; exit end;
val^:=round(i);
FromString:=True;
end;

function Ubyte.FromString(s:string):boolean;
var i:extended;
    e:integer;
begin
i:=0;
system.val(s,i,e);{ignore errors!}
if (e<>0)or(i<0)or(i>$ff) then
   begin FromString:=False; exit end;
val^:=round(i);
FromString:=True;
end;

function Ulongint.FromString(s:string):boolean;
var i:extended;
    e:integer;
begin
i:=0;
system.val(s,i,e);{ignore errors!}
if (e<>0)or(i<-MAXLONGINT)or(i>MAXLONGINT) then begin FromString:=False; exit end;
val^:=round(i);
FromString:=True;
end;

function Udouble.FromString(s:string):boolean;
var r:extended;
    e:integer;
begin
r:=0;
system.val(s,r,e);{ignore errors!}
if (e<>0)or(r<-MAXdouble)or(r>MAXdouble) then begin FromString:=False; exit end;
val^:=r;
FromString:=True;
end;

function Usingle.FromString(s:string):boolean;
var r:extended;
    e:integer;
begin
r:=0;
system.val(s,r,e);{ignore errors!}
if (e<>0)or(r<-MAXsingle)or(r>MAXsingle) then begin FromString:=False; exit end;
val^:=r;
FromString:=True;
end;

function Ustring.AsString:string;
begin
AsString:=val^;
end;

function Ustring.AsStringLen(l:word):string;
begin
if l>=length(val^) then AsStringLen:=val^
   else begin
   AsStringLen:=copy(val^,1,l-3)+'...';
   end;
end;

function Uboolean.AsString:string;
var s:string;
begin
if val^ then AsString:=TrueString
        else AsString:=FalseString;
end;

function Uinteger.AsString:string;
var s:string;
begin
str(val^,s);
AsString:=s;
end;

function Uword.AsString:string;
var s:string;
begin
str(val^,s);
AsString:=s;
end;

function Ubyte.AsString:string;
var s:string;
begin
str(val^,s);
AsString:=s;
end;

function Uenum.FromString(s:string):boolean;
type pttab=^ttab;
var  atab:pttab;
begin
FromString:=true;
atab:=tab;
case s[1] of
'0'..'9':FromString:=inherited FromString(s);
'-','<':dec(val^);
'+','>':inc(val^);
else begin
     val^:=0;
     while (s<>atab^[val^])and(val^<=n) do
                                        inc(val^);
     end;
end{case};
val^:=val^ mod n;
{if (val^<0)or(val^>=n) then
                       FromString:=false;}
end;

function Uenum.AsString:string;
type pttab=^ttab;
var  atab:pttab;
begin
atab:=tab;
if (val^<n)and(val^>=0) then AsString:=atab^[val^]
                        else AsString:='error';
end;

function Uenum.AsStringLen(l:word):string;
type pttab=^ttab;
var  atab:pttab;
begin
atab:=tab;
if (val^<n)and(val^>=0) then AsStringLen:=atab^[val^]
                        else AsStringLen:='error';
end;


function Uboolean.AsStringLen(l:word):string;
var pom:string;
    len:byte;
begin
if val^ then pom:='Yes/True/Tak'
        else pom:='No/False/Nie';
if byte(pom[0])>l then begin len:=l;pom[0]:=char(len);end;
AsStringLen:=pom;
end;

function Uinteger.AsStringLen(l:word):string;
var s:string;
begin
str(val^,s);
AsStringLen:=s;
end;

function Uword.AsStringLen(l:word):string;
var s:string;
begin
str(val^,s);
AsStringLen:=s;
end;

function Ubyte.AsStringLen(l:word):string;
var s:string;
begin
str(val^,s);
AsStringLen:=s;
end;

function Ulongint.AsString:string;
var s:string;
begin
str(val^,s);
AsString:=s;
end;

function Ulongint.AsStringLen(l:word):string;
var s:string;
begin
str(val^,s);
AsStringLen:=s;
end;

function Udouble.AsString:string;
var s:string;
begin
str(val^,s);
AsString:=s;
end;

function Udouble.AsStringLen(l:word):string;
var s:string;
    i:integer;
begin
i:=l;
repeat
str(val^:i,s);
dec(i);
until (length(s)<=l)or(i<=0);
AsStringLen:=s;
end;

function Usingle.AsString:string;
var s:string;
begin
str(val^,s);
AsString:=s;
end;

function Usingle.AsStringLen(l:word):string;
var s:string;
    i:integer;
begin
i:=l;
repeat
str(val^:i,s);
dec(i);
until (length(s)<=l)or(i<=0);
AsStringLen:=s;
end;


end.