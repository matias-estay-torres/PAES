@echo off
REM Compila paes_processor en Windows usando g++ (MSYS2/Mingw en PATH)
g++ -std=c++11 -O3 -fopenmp -Wall -Wextra -o paes_processor paes_processor_v2.cpp
if %ERRORLEVEL% neq 0 (
    echo Compilación fallida.
    pause
    exit /b %ERRORLEVEL%
)
echo Compilación exitosa: paes_processor.exe
