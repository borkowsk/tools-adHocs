@echo off
if "%2"=="" goto END
if "%1"=="" goto END
getuser
set name=%1
set id_number=%2
echo Exist %name% , %id_number% in passwd file ?
find "%name%" < passwd >obecnosc
if errorlevel==0 goto OK1
goto END
:OK1
find "%id_number%" < passwd >>obecnosc
if errorlevel==0 goto OK2
goto END
:OK2
type obecnosc
echo.
echo Are you shure for insert { %name% * %id_number% }
pause
attrib -r passwd
echo  %name% * %id_number% >> passwd 
attrib +r passwd
goto END2
:END
echo "  USAGE> add_user name id_number  "
:END2
