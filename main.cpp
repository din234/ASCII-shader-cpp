//#define _WIN32_WINNT 0x500 // window 2000 to later (= _WIN32_WINNT_WIN2K)

#pragma comment(lib, "User32.lib") // linker cl.exe GetAsynk
#define _WIN32_WINNT 0x501

// https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>

//#include <Windows.h>

#include "example/geometry.cpp"
#include "example/Motion2D.cpp"



void run(){
    //Motion mo;
    //mo.run();
    //imageRenderer render;
    //render.run();
    Geometry geo;
    geo.run();
}

void cls()
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}


void mouseHandle(MOUSE_EVENT_RECORD mouse){
    short x = mouse.dwMousePosition.X;
    short y = mouse.dwMousePosition.Y;
    printf("MOUSE X: %d;Y: %d; Button: %d   \n",x,y, mouse.dwButtonState);
}
/*
std::vector<int> inputQueue;
void keyEvent(KEY_EVENT_RECORD key){
    if (key.bKeyDown && mathExtra::checkElement(&inputQueue,(int)key.wVirtualKeyCode)){
        inputQueue.push_back(key.wVirtualKeyCode);
    } else if (key.bKeyDown == 0){
        mathExtra::deleteElement(&inputQueue,(int)key.wVirtualKeyCode);
    }

    
    for (int i = 0; i < inputQueue.size();i++){
        printf("%d ",inputQueue[i]);
    }
}

void inputBuffer(){
    HANDLE hInput;//,hOutput;
    INPUT_RECORD ckey;
    DWORD read,mode,k;

    hInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS |ENABLE_MOUSE_INPUT);

    bool test = true;
    while (test){
        if (GetAsyncKeyState(VK_ESCAPE)){test = false;};
        if (!ReadConsoleInput(hInput,&ckey,1,&read)){
            printf("FAILED");
        };
        //system("cls");
        cls();
        switch(ckey.EventType){
            case KEY_EVENT:
                //printf("%d",read);
                keyEvent(ckey.Event.KeyEvent);break;
            case MOUSE_EVENT: mouseHandle(ckey.Event.MouseEvent);break;
            case WINDOW_BUFFER_SIZE_EVENT: printf("SIZE");break;
            case 16: printf("START");break;
        }
        //system("pause");
    }
}*/


int main(){
    //inputBuffer();
    run();
    _CrtDumpMemoryLeaks();
    //std::cin.ignore();
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    return 0;
}
