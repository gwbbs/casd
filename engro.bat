@echo off
cls
gcc -o ciao .\ciao.c -lpdcurses
if %errorlevel% neq 0 (
    echo Compilazione Fallita.
    pause
) else (
    .\ciao.exe
)