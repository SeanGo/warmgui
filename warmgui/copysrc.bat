echo off

del %1-%2.rar

rem if "%2"=="win7" echo "i am in win7"
rem if "%2"=="win7" set dstdir="F:\source_backup\warmgui"

rem if "%2"=="2008" echo "i am in Win2008ServerR2"
rem if "%2"=="2008" set dstdir="g:\tk\chan"

set my_dir=%1
set dstdir=H:\source_backup\%1

cd ..
xcopy /T /E %my_dir% %1-%2
cd %my_dir%

REM delete bin
rd ..\%1-%2\bin /S/Q

REM delete lib
rd ..\%1-%2\lib /S/Q

REM delete release
rd ..\%1-%2\release /S/Q

REM delete Debug
rd ..\%1-%2\Debug


REM Copy effect
set target=effect
copy %target% ..\%1-%2\%target%\

REM Copy console
set target=console
copy %target% ..\%1-%2\%target%\
copy %target%\res ..\%1-%2\%target%\res\
copy %target%\Resource ..\%1-%2\%target%\Resource\
rd ..\%1-%2\%target%\Debug /S/Q
rd ..\%1-%2\%target%\Release /S/Q
rd ..\%1-%2\%target%\x64 /S/Q
del ..\%1-%2\%target%\*.user
del ..\%1-%2\%target%\*.aps

REM Copy conveter
set target=conveter
copy %target% ..\%1-%2\%target%\
copy %target%\res ..\%1-%2\%target%\res\
copy %target%\Resource ..\%1-%2\%target%\Resource\
rd ..\%1-%2\%target%\Debug /S/Q
rd ..\%1-%2\%target%\Release /S/Q
rd ..\%1-%2\%target%\x64 /S/Q
del ..\%1-%2\%target%\*.user
del ..\%1-%2\%target%\*.aps


REM Copy ctpmdif
set target=ctpmdif
copy %target% ..\%1-%2\%target%\
copy %target%\res ..\%1-%2\%target%\res\
copy %target%\Resource ..\%1-%2\%target%\Resource\
rd ..\%1-%2\%target%\Debug /S/Q
rd ..\%1-%2\%target%\Release /S/Q
rd ..\%1-%2\%target%\x64 /S/Q
del ..\%1-%2\%target%\*.user
del ..\%1-%2\%target%\*.aps

REM Copy ctptdif
set target=ctptdif
copy %target% ..\%1-%2\%target%\
copy %target%\res ..\%1-%2\%target%\res\
copy %target%\Resource ..\%1-%2\%target%\Resource\
rd ..\%1-%2\%target%\Debug /S/Q
rd ..\%1-%2\%target%\Release /S/Q
rd ..\%1-%2\%target%\x64 /S/Q
del ..\%1-%2\%target%\*.user
del ..\%1-%2\%target%\*.aps

REM Copy euclid
set target=euclid
copy %target% ..\%1-%2\%target%\
copy %target%\res ..\%1-%2\%target%\res\
copy %target%\Resource ..\%1-%2\%target%\Resource\
rd ..\%1-%2\%target%\Debug /S/Q
rd ..\%1-%2\%target%\Release /S/Q
rd ..\%1-%2\%target%\x64 /S/Q
del ..\%1-%2\%target%\*.user
del ..\%1-%2\%target%\*.aps

REM Copy FutureTeller
set target=FutureTeller
copy %target% ..\%1-%2\%target%\
copy %target%\res ..\%1-%2\%target%\res\
copy %target%\Resource ..\%1-%2\%target%\Resource\
rd ..\%1-%2\%target%\Debug /S/Q
rd ..\%1-%2\%target%\Release /S/Q
rd ..\%1-%2\%target%\x64 /S/Q
del ..\%1-%2\%target%\*.user
del ..\%1-%2\%target%\*.aps

REM Copy learndx2d
set target=learndx2d
copy %target% ..\%1-%2\%target%\
copy %target%\res ..\%1-%2\%target%\res\
copy %target%\Resource ..\%1-%2\%target%\Resource\
rd ..\%1-%2\%target%\Debug /S/Q
rd ..\%1-%2\%target%\Release /S/Q
rd ..\%1-%2\%target%\x64 /S/Q
del ..\%1-%2\%target%\*.user
del ..\%1-%2\%target%\*.aps

REM Copy test_dxwindow
set target=test_dxwindow
copy %target% ..\%1-%2\%target%\
copy %target%\res ..\%1-%2\%target%\res\
copy %target%\Resource ..\%1-%2\%target%\Resource\
rd ..\%1-%2\%target%\Debug /S/Q
rd ..\%1-%2\%target%\Release /S/Q
rd ..\%1-%2\%target%\x64 /S/Q
del ..\%1-%2\%target%\*.user
del ..\%1-%2\%target%\*.aps

REM Copy test_trade
set target=test_trade
copy %target% ..\%1-%2\%target%\
copy %target%\res ..\%1-%2\%target%\res\
copy %target%\Resource ..\%1-%2\%target%\Resource\
rd ..\%1-%2\%target%\Debug /S/Q
rd ..\%1-%2\%target%\Release /S/Q
rd ..\%1-%2\%target%\x64 /S/Q
del ..\%1-%2\%target%\*.user
del ..\%1-%2\%target%\*.aps

REM Copy TestCtpWnd
set target=TestCtpWnd
copy %target% ..\%1-%2\%target%\
copy %target%\res ..\%1-%2\%target%\res\
copy %target%\Resource ..\%1-%2\%target%\Resource\
rd ..\%1-%2\%target%\Debug /S/Q
rd ..\%1-%2\%target%\Release /S/Q
rd ..\%1-%2\%target%\x64 /S/Q
del ..\%1-%2\%target%\*.user
del ..\%1-%2\%target%\*.aps

REM Copy TestD2D
set target=TestD2D
copy %target% ..\%1-%2\%target%\
copy %target%\res ..\%1-%2\%target%\res\
copy %target%\Resource ..\%1-%2\%target%\Resource\
rd ..\%1-%2\%target%\Debug /S/Q
rd ..\%1-%2\%target%\Release /S/Q
rd ..\%1-%2\%target%\x64 /S/Q
del ..\%1-%2\%target%\*.user
del ..\%1-%2\%target%\*.aps

REM Copy utility
set target=utility
copy %target% ..\%1-%2\%target%\
copy %target%\res ..\%1-%2\%target%\res\
copy %target%\Resource ..\%1-%2\%target%\Resource\
rd ..\%1-%2\%target%\Debug /S/Q
rd ..\%1-%2\%target%\Release /S/Q
rd ..\%1-%2\%target%\x64 /S/Q
del ..\%1-%2\%target%\*.user
del ..\%1-%2\%target%\*.aps

REM Copy warmgui
set target=warmgui
copy %target% ..\%1-%2\%target%\
copy %target%\res ..\%1-%2\%target%\res\
copy %target%\Resource ..\%1-%2\%target%\Resource\
copy %target%\dxui ..\%1-%2\%target%\dxui\
rd ..\%1-%2\%target%\Debug /S/Q
rd ..\%1-%2\%target%\Release /S/Q
rd ..\%1-%2\%target%\x64 /S/Q
del ..\%1-%2\%target%\*.user
del ..\%1-%2\%target%\*.aps


REM Copy bat files
copy copysrc.bat ..\%1-%2\
copy %1.sln ..\%1-%2\

cd ..

winrar.exe a %1-%2.rar %1-%2

copy %1-%2.rar %dstdir%

rd %1-%2 /S/Q

del %1-%2.rar

cd %my_dir%

