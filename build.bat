@echo off

call "%VSINSTALLDIR%\VC\Auxiliary\Build\vcvarsall.bat" x64

cl /EHsc /Zi /Fe: main.exe main.cpp