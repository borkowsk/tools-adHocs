@ECHO Ten plik wsadowy wymaga jednoliterowego parametru oznaczajacego model pamieci
@if "%1"=="" exit
@SET OLDPATH=%PATH%
@ECHO OLD PATH IS %OLDPATH%
@SET PATH=C:\BR\BIN
@echo Kompilacja do OBJ(-c);Nested comment(-C);
@echo W modelu danym palametrem(-m%1);I z UNsigned char(-K) 
@echo Uzytkowanie pamieci extended(-Qx);
@BCC -m%1 -Qx -Z -d -K -c -C BAS*.CPP
@echo Przygotowanie biblioteki
del baslib%1.lib
@FOR %%I IN (BAS*.OBJ) DO TDSTRIP %%I
@FOR %%I IN (BAS*.OBJ) DO TLIB baslib%1 /C +%%I , BASLIB%1.LST
@echo Usuwanie *.obj
del BAS*.obj
@SET PATH=%OLDPATH%
@SET OLDPATH=
