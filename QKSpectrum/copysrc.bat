echo off

del %1-%2.rar

rem if "%2"=="win7" echo "i am in win7"
rem if "%2"=="win7" set dstdir="H:\source_backup\warmgui"

rem if "%2"=="2008" echo "i am in Win2008ServerR2"
rem if "%2"=="2008" set dstdir="g:\tk\chan"

set my_dir=%1
set dstdir=H:\source_backup\%1
set github=D:\github\warmgui

cd ..
xcopy /T /E %my_dir% %1-%2
xcopy /T /E %my_dir% %github% /y

cd %my_dir%

REM delete bin
rd ..\%1-%2\bin /S/Q

REM delete lib
rd ..\%1-%2\lib /S/Q

REM delete release
rd ..\%1-%2\release /S/Q

REM delete Debug
rd ..\%1-%2\Debug

rd %github%\ipch /S/Q
rd %github%\Lib /S/Q
rd %github%\Release /S/Q
rd %github%\Debug /S/Q


REM Copy QKSpectrum
set target=QKSpectrum
copy %target% ..\%1-%2\%target%\
copy %target%\res ..\%1-%2\%target%\res\
copy %target%\shader ..\%1-%2\%target%\shader\
copy %target%\Resource ..\%1-%2\%target%\Resource\
rd ..\%1-%2\%target%\Debug /S/Q
rd ..\%1-%2\%target%\Release /S/Q
rd ..\%1-%2\%target%\x64 /S/Q
del ..\%1-%2\%target%\*.user
del ..\%1-%2\%target%\*.aps
rd %github%\%target%\Debug /S/Q
rd %github%\%target%\Release /S/Q
copy %target% %github%\%target%\ /y
copy %target%\res %github%\%target%\res\ /y
copy %target%\shader %github%\%target%\shader\ /y
copy %target%\Resource %github%\%target%\Resource\ /y
del %github%\%target%\*.user
del %github%\%target%\*.filters

REM Copy warmgui
set target=warmgui
copy %target% ..\%1-%2\%target%\
copy %target%\res ..\%1-%2\%target%\res\
copy %target%\Resource ..\%1-%2\%target%\Resource\
rd ..\%1-%2\%target%\Debug /S/Q
rd ..\%1-%2\%target%\Release /S/Q
rd ..\%1-%2\%target%\x64 /S/Q
del ..\%1-%2\%target%\*.user
del ..\%1-%2\%target%\*.aps
rd %github%\%target%\Debug /S/Q
rd %github%\%target%\Release /S/Q
copy %target% %github%\%target%\ /y
copy %target%\res %github%\%target%\res\ /y
copy %target%\Resource %github%\%target%\Resource\ /y
del %github%\%target%\*.user
del %github%\%target%\*.filters


REM Copy bat files
copy copysrc.bat ..\%1-%2\
copy requer.bat  ..\%1-%2\
copy %1.sln ..\%1-%2\
copy %1.sln %github%\ /y

cd ..

winrar.exe a %1-%2.rar %1-%2

copy %1-%2.rar %dstdir%

rd %1-%2 /S/Q

del %1-%2.rar

cd %my_dir%

