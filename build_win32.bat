@echo OFF

SETLOCAL

REM call "E:\Windows Development Tools\Visual Studio\VC\Auxiliary\Build\vcvarsall.bat" x64

set pdir=%~dp0%
set CC=clang
set CFLAGS=-std=c11 -g -Wall -Wpedantic -Wno-unused-parameter -Wno-switch -Wno-unused-function
set CFLAGS=%CFLAGS% -Wno-deprecated-declarations
REM Show command but dont compile
REM set CFLAGS=%CFLAGS% -###
set CFLAGS=%CFLAGS% -I%pdir%dependencies\glfw\include
set CFLAGS=%CFLAGS% -I%pdir%dependencies\freetype\release_win32\include
set CFLAGS=%CFLAGS% -I%pdir%dependencies\glad\include
set CFLAGS=%CFLAGS% -MJ %pdir%compile_commands.json
set CFLAGS=%CFLAGS% -o textus.exe

set LINKFLAGS=-lglfw3
set LINKFLAGS=%LINKFLAGS% -lfreetype
set LINKFLAGS=%LINKFLAGS% -lopengl32
set LINKFLAGS=%LINKFLAGS% -lshell32
set LINKFLAGS=%LINKFLAGS% -lgdi32
set LINKFLAGS=%LINKFLAGS% -luser32
set LINKFLAGS=%LINKFLAGS% -lkernel32
set LINKFLAGS=%LINKFLAGS% -L%pdir%dependencies\glfw\win32\lib-vc2022
REM set LINKFLAGS=%LINKFLAGS% -L%pdir%\dependencies\glfw\win32\lib-mingw-w64
set LINKFLAGS=%LINKFLAGS% -L%pdir%dependencies\freetype\release_win32\release_static\vs2015-2022\win64

set CFILES=%pdir%\dependencies\glad\src\glad.c
pushd ".\code"
for /R %%c in (*.cpp) do call set %CC%CFILES=%%c %%CFILES%%
for /R %%c in (*.c) do call set CFILES=%%c %%CFILES%%
popd

IF NOT EXIST "bin" mkdir bin

pushd "bin"
%CC% %CFLAGS% %cfiles% %LINKFLAGS%
popd

set compilecommands=
for /f %%i in ('echo [') do call set compilecommands=%%i
for /f %%i in ('cat %pdir%compile_commands.json') do call set compilecommands=%%compilecommands%% %%i
for /f %%i in ('echo ]') do call set compilecommands=%%compilecommands%% %%i


REM echo %compilecommands% > compile_commands.json
echo %compilecommands%

REM.\bin\textus.exe

ENDLOCAL
