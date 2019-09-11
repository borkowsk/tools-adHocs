unit multitype;
interface
type multiroot=object { Przodek uni do przechowywania dowolnej wartosci }
     destructor done; virtual ;
     function FromString(s:string):boolean;virtual;
     function Fromdouble(r:double):boolean;    virtual;
     function FromInt(i:longint):boolean;  virtual;
     function AsString:string;             virtual;
     function AsStringLen(l:word):string;  virtual;
     function AsInteger:longint;           virtual;
     function Asdouble:double;                 virtual;
     function AsFloat:extended;            virtual;
     end;
implementation
destructor multiroot.done;
begin
writeln('Abstract type multiroot used!');halt(1);
end;
function multiroot.FromString(s:string):boolean;
begin
writeln('Abstract type multiroot used!');halt(1);
end;

function multiroot.Fromdouble(r:double):boolean;
begin
writeln('Abstract type multiroot used!');halt(1);
end;

function multiroot.FromInt(i:longint):boolean;
begin
writeln('Abstract type multiroot used!');halt(1);
end;

function multiroot.AsString:string;
begin
writeln('Abstract type multiroot used!');halt(1);
end;

function multiroot.AsStringLen(l:word):string;
begin
writeln('Abstract type multiroot used!');halt(1);
end;

function multiroot.AsInteger:longint;
begin
writeln('Abstract type multiroot used!');halt(1);
end;

function multiroot.Asdouble:double;
begin
writeln('Abstract type multiroot used!');halt(1);
end;

function multiroot.AsFloat:extended;
begin
writeln('Abstract type multiroot used!');halt(1);
end;

end.

