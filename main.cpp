//#define _WIN32_WINNT 0x500 // window 2000 to later (= _WIN32_WINNT_WIN2K)

#pragma comment(lib, "User32.lib") // linker user interface
#pragma comment(lib, "Gdi32.lib") // linker for drawing function
#define _WIN32_WINNT 0x500

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <Windows.h>


#include "example/imageRenderer.cpp"
#include "example/geometry.cpp"
#include "example/Motion2D.cpp"

int main(){
    
    //inputBuffer();

    /*
    HDC hdc = GetDC(0);
    GLYPHMETRICS letter;

    MAT2 m2 = {{0, 1}, {0, 0}, {0, 0}, {0, 1}}; // identity matrix
    DWORD cbBuf = GetGlyphOutline(hdc, 'a', GGO_BITMAP, &letter, 0, NULL, &m2);
    
    printf("%i \n", cbBuf);*/

    {
        //Motion mo;
        //mo.run();
        //imageRenderer render;
        //render.run();
        //Geometry geo;
        //geo.run();
    }

    system("pause");
    _CrtDumpMemoryLeaks();
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    return 0;
}
