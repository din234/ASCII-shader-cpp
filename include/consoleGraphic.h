
// #define _WIN32_WINNT 0x500 // window 2000 to later (= _WIN32_WINNT_WIN2K)

// header guard
#ifndef CONSOLE_GRAPHIC_H
#define CONSOLE_GRAPHIC_H


#include <Windows.h>
#include <string>
#include <thread>


/*
// Add this for compile using g++
#include "mingw_thread/mingw.thread.h" // mingW

typedef struct _CONSOLE_FONT_INFOEX {
    ULONG cbSize;
    DWORD nFont;
    COORD dwFontSize;
    UINT  FontFamily;
    UINT  FontWeight;
    WCHAR FaceName[LF_FACESIZE];
} CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;

#ifdef __cplusplus
extern "C" { // add basic c libary
#endif
BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow,PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx
);
#ifdef __cplusplus
}
#endif*/


class consoleGraphic {
    private:
        HWND consoleWindow = GetConsoleWindow();
        HANDLE hout;
        CONSOLE_FONT_INFOEX fontInfo{0};
        CHAR_INFO *map;
        COORD buffSize;
        COORD buffCord;
        SMALL_RECT writeArea;

        CHAR lightScale[5] = {' ','.','-','>','@'};
    protected:
        short buffWidth,buffHeight;
        const int minTimePerFrame = 33; // ms
        int elappsedTime = 0;
        bool debug = true;

    protected:
        void createBuffering(int fontSize,short setWidth, short setHeight);
        virtual int onCreate();
        virtual int onUpdate();


    protected:
        void writeV2(SHORT frame); // version 2
        void setFont(int x, int y,int lightIndex,int attribute);
    public:
        consoleGraphic();
        ~consoleGraphic(){ // prevent memory leak
            delete[] map;
        }
        void run();
};









consoleGraphic::consoleGraphic(){
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
    fontInfo.cbSize=sizeof(fontInfo);
    hout = GetStdHandle(STD_OUTPUT_HANDLE);
    buffCord = {0,0}; // DEFAULT
    buffWidth = 0;
    buffHeight = 0;

    //CONSOLE_SCREEN_BUFFER_INFO scbinfo;
    //GetConsoleScreenBufferInfo(hout,&scbinfo);
    //scbinfo.dwSize.X = scbinfo.dwMaximumWindowSize.X;
    //scbinfo.dwSize.Y = scbinfo.dwMaximumWindowSize.Y;
    //SetConsoleScreenBufferSize(hout,scbinfo.dwSize);
    createBuffering(5,120,120);
    
}

int consoleGraphic::onCreate(){
    return 0;
}

int consoleGraphic::onUpdate(){
    return 0;
}

void consoleGraphic::createBuffering(int fontSize,short setWidth, short setHeight){
    delete[] map;
    if (fontSize != 0 && setWidth > 0 && setHeight > 0){
        COORD dwSize = {setWidth,setHeight};
        buffWidth = setWidth;
        buffHeight = setHeight;
        fontInfo.dwFontSize.X = fontSize;
        fontInfo.dwFontSize.Y = fontSize;
        
        SetCurrentConsoleFontEx(hout,true,&fontInfo);
        map = new CHAR_INFO[buffWidth*buffHeight];
        buffSize = {buffWidth,buffHeight};
        writeArea = {0,0,(short)(buffWidth-1),(short)(buffHeight-1)};
        
        for (int i = 0; i < buffWidth*buffHeight; i++){
            map[i].Char.AsciiChar = lightScale[0];
            map[i].Attributes = 0;
        }
        SetConsoleScreenBufferSize(hout,dwSize);
        SetConsoleWindowInfo(hout,true,&writeArea);
    }
}

void consoleGraphic::writeV2(SHORT frame){
    if (map != NULL){
        SHORT temp = frame * 100;
        writeArea.Top = temp;
        //writeArea = {temp,10,(short)(width-1),(short)(height-1)};
        //buffSize = {200,20};
        //buffCord = {30,50};
        WriteConsoleOutput(hout,map,buffSize,buffCord,&writeArea);
    }
}


void consoleGraphic::setFont(int x, int y,int lightIndex,int attribute){
    if (map != NULL && x < buffWidth && x > 0 && y > 0 && y < buffHeight && lightIndex < sizeof(lightScale)/sizeof(lightScale[0])){
        map[x+y*buffWidth].Char.AsciiChar = lightScale[lightIndex];
        map[x+y*buffWidth].Attributes = attribute;
    }
}

void consoleGraphic::run(){
    onCreate();
    
    std::thread th([=]()
    {
        while (true){
            if (debug){
                debug = false;
                std::string title = "Elappsed Time: " + std::to_string(elappsedTime);
                SetConsoleTitleA(title.c_str());
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    while (true){
        auto start = std::chrono::system_clock::now();
        onUpdate();
        WriteConsoleOutput(hout,map,buffSize,buffCord,&writeArea);
        auto end = std::chrono::system_clock::now();
        auto elappsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        elappsedTime = elappsed.count();
        debug = true;

        if (elappsedTime < minTimePerFrame){
            std::this_thread::sleep_for(std::chrono::milliseconds(minTimePerFrame - elappsedTime));
        }
    }
}

#endif // CONSOLE_GRAPHIC_H