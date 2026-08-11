@echo off
REM Links this repo's sublime\User into Sublime Text's config directory.
REM Run as Administrator, or enable Windows Developer Mode (for symlinks).
REM   setup.bat                      -> installed Sublime Text
REM   setup.bat "D:\SublimeText\Data" -> portable install
setlocal
set "REPO=%~dp0"
if "%~1"=="" (
    set "TARGET=%APPDATA%\Sublime Text\Packages\User"
) else (
    set "TARGET=%~1\Packages\User"
)

if exist "%TARGET%" (
    echo Backing up existing config...
    move "%TARGET%" "%TARGET%.backup" >nul 2>&1
    rmdir "%TARGET%" >nul 2>&1
)

mklink /D "%TARGET%" "%REPO%sublime\User"
if errorlevel 1 (
    echo.
    echo Symlink failed. Run this file as Administrator, or turn on
    echo Settings ^> System ^> For developers ^> Developer Mode.
) else (
    echo Linked: "%TARGET%"  -^>  "%REPO%sublime\User"
    echo Open Sublime, then Project ^> Open Project... ^> "%REPO%cp.sublime-project"
)
pause
