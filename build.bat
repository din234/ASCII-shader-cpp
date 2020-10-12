@echo off

call "%VSINSTALLDIR%\VC\Auxiliary\Build\vcvarsall.bat" x64

cl /EHsc /MDd /Zi /Fe: main.exe main.cpp