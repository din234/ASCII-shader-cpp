@echo off

call "%VSINSTALLDIR%\VC\Auxiliary\Build\vcvarsall.bat" x64


rem cl testChild.cpp
cl /EHsc /MDd /Zi /Fe: main.exe main.cpp


rem nasm -f elf64 foo.S
rem g++ main.cpp foo.obj -o main
rem cl /EHsc main.cpp foo.obj