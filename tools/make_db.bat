@echo off
rem Builds localizer.dat from strings\strings_<lang>.txt using Draan's Paxor.
rem Usage: make_db.bat [lang]     (default: tr  ->  ..\strings\strings_tr.txt)
setlocal
set LANGCODE=%1
if "%LANGCODE%"=="" set LANGCODE=tr
if not exist "%~dp0..\strings\strings_%LANGCODE%.txt" (
  echo strings\strings_%LANGCODE%.txt not found
  exit /b 1
)
copy /y "%~dp0..\strings\strings_%LANGCODE%.txt" "%~dp0strings.txt" >nul
pushd "%~dp0"
Paxor.exe
popd
move /y "%~dp0localizer.dat" "%~dp0..\dist\localizer_%LANGCODE%.dat" >nul
del "%~dp0strings.txt"
echo Written dist\localizer_%LANGCODE%.dat
