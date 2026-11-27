@echo off
rem Build Piano Simulator with the compiler that ships with the raylib Windows installer.
rem Set RAYLIB_DIR if raylib is not installed at C:\raylib.

if "%RAYLIB_DIR%"=="" set RAYLIB_DIR=C:\raylib
set PATH=%RAYLIB_DIR%\w64devkit\bin;%PATH%

if not exist build mkdir build

gcc src\main.c -o build\piano.exe ^
    -std=c99 -Wall -Wextra -O2 ^
    -I"%RAYLIB_DIR%\raylib\src" -L"%RAYLIB_DIR%\raylib\src" ^
    -lraylib -lopengl32 -lgdi32 -lwinmm

if errorlevel 1 exit /b 1
echo Built build\piano.exe
