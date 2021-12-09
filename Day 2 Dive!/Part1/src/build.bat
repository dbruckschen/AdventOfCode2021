@echo off
cls
set BUILDTOOLS="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" 
call %BUILDTOOLS%

cl.exe /Zi main.c /Fe: practice.exe 

exit

