@echo off
REM setlocal EnableDelayedExpansion

:: Path
set Tools=..\..\tools
set MSXtk=%Tools%\MSXtk\bin
set Dest=.\content

if not exist %Dest% md %Dest%

echo Building image data...

%MSXtk%\MSXimg.exe datasrc\test.png -out %Dest%\test.h -pos 0 192 -size 8 8 -num 8 3 -name g_DataBackground -mode sprt -l i8 0 0 1 1 0xFFFFFF

pause