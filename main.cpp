//#define _WIN32_WINNT 0x500 // window 2000 to later (= _WIN32_WINNT_WIN2K)

#pragma comment(lib, "User32.lib") // linker user interface
#pragma comment(lib, "Gdi32.lib") // GDI graphic
#pragma comment(lib, "Shell32.lib") // shell console

#pragma comment(lib, "Advapi32.lib") // Registry

#define _WIN32_WINNT 0x500

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


#define STRICT

#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <tchar.h>
#include <assert.h>
// extern "C" int bar(int param);

#include "example/geometry.cpp"
//#include "example/Motion2D.cpp"
//#include "example/sortAlgorithm.cpp"
#include "example/gradientDecent.cpp"
#include "example/imagerenderer.cpp"

/*
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE,LPSTR lpCmd, int nShow)
{
    HINSTANCE hRslt = ShellExecute(NULL, szOperation,
    szAddress, NULL, NULL, SW_SHOWNORMAL);
    //assert( hRslt > (HINSTANCE) HINSTANCE_ERROR);
    return 0;
}*/

// const TCHAR szOperation[] = _T("open");
// const TCHAR szAddress[] = _T("www.google.com");




/*
void fontRegistry(){
    const TCHAR keyFont[] = _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts");
    HKEY hKey;

    RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyFont,0,KEY_READ,&hKey);

    for (int i = 0;; i++){
        TCHAR valueName[MAX_PATH];
        BYTE valueData[MAX_PATH];
        DWORD valueLength = MAX_PATH;
        DWORD dataLength = MAX_PATH;
        DWORD dwType;
        if (RegEnumValue(hKey,i,valueName,&valueLength,NULL,&dwType,valueData,&dataLength) != 
            ERROR_SUCCESS){
        break;
        }
        printf("%i. %s ===== %s \n",i,valueName,valueData);
    }
    RegCloseKey(hKey);
    //std::cout << hKey << std::endl;
}
*/



int main(){
    {
        Geometry test;
        test.run();
    }
    system("pause");
    _CrtDumpMemoryLeaks(); // to check memory leak
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    return 0;
}
