//#define _WIN32_WINNT 0x500 // window 2000 to later (= _WIN32_WINNT_WIN2K)

#pragma comment(lib, "User32.lib") // linker cl.exe GetAsynk
#define _WIN32_WINNT 0x501

// https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <Windows.h>
#include <iostream>

#include "example/imageRenderer.cpp"
#include "example/geometry.cpp"



void run(){
    //imageRenderer render;
    //render.run();
    Geometry geo;
    geo.run();
}


int main(){
    run();
    //_CrtDumpMemoryLeaks();
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    return 0;
}
