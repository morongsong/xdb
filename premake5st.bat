
@echo off
cd /d %~dp0
echo %CD%

premake5 vs2015

pause

cd /d _Build
for %%a in ("*.sln") DO (set BUILD_TARGET="%%a")

echo %BUILD_TARGET%
call "%VS140COMNTOOLS%\vsvars32.bat"

msbuild %BUILD_TARGET% /p:Configuration=Debug;Platform=Win32
msbuild %BUILD_TARGET% /p:Configuration=Release;Platform=Win32
msbuild %BUILD_TARGET% /p:Configuration=Debug;Platform=x64
msbuild %BUILD_TARGET% /p:Configuration=Release;Platform=x64
