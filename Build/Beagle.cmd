@echo off
set BEAGLE_BUILD_PATH=%~dp0%
set BEAGLE_BUILD_PATH=%BEAGLE_BUILD_PATH:~0,-1%
set _NTROOT=%~p0%
set _NTROOT=%_NTROOT:\Build\=%
set _NTDRIVE=%~d0%
rem set TOOLS_PATH=c:\cygwin\bin;c:\CodeSourcery\Sourcery_G++_Lite\bin;%BEAGLE_BUILD_PATH%\amd64;%BEAGLE_BUILD_PATH%\x86
set TOOLS_PATH=%BEAGLE_BUILD_PATH%\amd64;%BEAGLE_BUILD_PATH%\x86;%BEAGLE_BUILD_PATH%\internal\amd64 
set NTMAKEENV=%BEAGLE_BUILD_PATH%\config
set BASEDIR=%_NTDRIVE%%_NTROOT%
set USE_OBJECT_ROOT=1

doskey /MACROFILE=%BASEDIR%\Build\aliases.txt

set SDK_INC_PATH=%BASEDIR%\Build\sdk\inc
set SDK_LIB_PATH=%BASEDIR%\Build\sdk\lib\*
set CRT_INC_PATH=%BASEDIR%\Build\vc\inc
set CRT_LIB_PATH=%BASEDIR%\Build\vc\lib\*

set BUILD_OPTIONS=/e /M %NUMBER_OF_PROCESSORS% /E /g

if /i "%1" == "arm" (
    set BUILD_DEFAULT_TARGETS=-arm
    set _BUILDARCH=ARM
    set ARM=1
    set TOOLS_PATH=%TOOLS_PATH%;%BEAGLE_BUILD_PATH%\vc\x86_arm
)

if /i "%1"=="amd64" (
    set BUILD_DEFAULT_TARGETS=-amd64
    set _BUILDARCH=AMD64
    set AMD64=1
    set TOOLS_PATH=%TOOLS_PATH%;%BEAGLE_BUILD_PATH%\vc\amd64
)

set OBJECT_ROOT=%BASEDIR%.obj.%_BUILDARCH%
set _NTTREE=%BASEDIR%.binaries.%_BUILDARCH%

set PATH=%TOOLS_PATH%;%PATH%

%_NTDRIVE%
cd %_NTROOT%


