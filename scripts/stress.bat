@echo off
REM Usage: ..\..\scripts\stress.bat A brute 1000
setlocal enabledelayedexpansion
set SOL=%1
set BRUTE=%2
set ITER=%3
if "%SOL%"=="" set SOL=A
if "%BRUTE%"=="" set BRUTE=brute
if "%ITER%"=="" set ITER=1000

g++ -std=gnu++20 -O2 -DLOCAL -o %SOL%.exe %SOL%.cpp || exit /b 1
g++ -std=gnu++20 -O2 -DLOCAL -o %BRUTE%.exe %BRUTE%.cpp || exit /b 1

for /L %%i in (1,1,%ITER%) do (
    python gen.py %%i > _in.txt
    %SOL%.exe   < _in.txt > _out1.txt
    %BRUTE%.exe < _in.txt > _out2.txt
    fc /W _out1.txt _out2.txt > nul || (
        echo MISMATCH on test %%i
        type _in.txt
        echo --- %SOL% ---   & type _out1.txt
        echo --- %BRUTE% --- & type _out2.txt
        copy /y _in.txt input.txt > nul
        exit /b 1
    )
    echo passed %%i/%ITER%
)
echo All %ITER% tests passed.
