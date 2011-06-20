@echo off
if "%TCSYSTEM_SETUP_DONE%" == "1" goto end

if "%1" == "" goto failed
if "%1" == "32" goto 32bit
if "%1" == "64" goto 64bit
goto failed

:32bit
set WIN_PLATFORM=Win32
goto do

:64bit
set WIN_PLATFORM=x64
goto do

:do
echo ----------------------------------------------------------------
echo                         Setting up path settings
set TCSYSTEM_DIR=%CD%
set TCSYSTEM_LIB_DIR=%TCSYSTEM_DIR%\bin\%WIN_PLATFORM%

SET INCLUDE=%INCLUDE%;"%TCSYSTEM_DIR%\inc"
SET INCLUDE=%INCLUDE%;"%TCSYSTEM_DIR%\Modules\GUI"
SET INCLUDE=%INCLUDE%;"%TCSYSTEM_DIR%\Modules\OpenGL"

SET LIB=%LIB%;%TCSYSTEM_LIB_DIR%

set TCSYSTEM_SETUP_DONE=1
goto end

:failed
cd %TCSYSTEM_DIR%
echo ----------------------------------------------------------------
echo Setup failed
echo ----------------------------------------------------------------
set TCSYSTEM_SETUP_DONE=0

:end
