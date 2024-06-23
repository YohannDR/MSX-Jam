@echo off
REM setlocal EnableDelayedExpansion

:: Path
set Tools=..\..\tools
set MSXtk=%Tools%\MSXtk\bin
set Dest=.\content

if not exist %Dest% md %Dest%

echo Building image data...

%MSXtk%\MSXimg.exe datasrc\tileset.png -out %Dest%\tileset.h -pos 0 0 -size 0 0 -num 8 3 -name sTilesetGfx -mode gm2 -l i8 0 0 1 1 0xFFFFFF

pause