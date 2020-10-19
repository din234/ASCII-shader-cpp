
// #define _WIN32_WINNT 0x500 // window 2000 to later (= _WIN32_WINNT_WIN2K)

// header guard
#ifndef CONSOLE_GRAPHIC_H
#define CONSOLE_GRAPHIC_H


#include <Windows.h>
#include <string>
#include <cmath>
#include <thread>

#include "include/process.h"

#define BLACK 0x0000
#define BLUE 0x0001
#define GREEN 0x0002
#define RED 0x0004
#define WHITE 0x000f


enum greyLevel {
    NONE = ' ',
    LOW = '.',
    MEDIUM = '-',
    HIGH = '>',
    FULL = '@',
};
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


typedef struct PIXEL_ATTR {
    int greyLevel = 0;
    int color = 0;
};


class ConsoleGraphic {
    private:
        HWND consoleWindow = GetConsoleWindow();
        HANDLE hout;
        CONSOLE_FONT_INFOEX fontInfo{0};
        CHAR_INFO *map;
        COORD buffSize;
        COORD buffCord;
        SMALL_RECT writeArea;
        /*
        enum greyLevel {
            NONE = ' ',
            LOW = '.',
            MEDIUM = '-',
            HIGH = '>',
            FULL = '@',
        };*/
        CHAR lightScale[5] = {' ','.','-','>','@'};
        processTest test;
    protected:
        short buffWidth,buffHeight;
        const int minTimePerFrame = 33; // ms
        int elappsedTime = 0;
        bool runTest = true;

    private:
        void frameT();
    protected:
        void createBuffering(int fontSize,short setWidth, short setHeight);
        virtual int onCreate();
        virtual int onUpdate();
        virtual std::string logger();

    protected:
        void writeV2(SHORT frame); // version 2
        void setFont(int x, int y,PIXEL_ATTR attr);
        void drawLine(float x1,float y1,float x2,float y2,PIXEL_ATTR lineAttr);

        void drawRect(int x, int y, int width, int height,PIXEL_ATTR borderAttr);
        void drawRect(int x, int y, int width, int height,PIXEL_ATTR borderAttr,PIXEL_ATTR fillAttr);

        void drawPoly();
        void cls();
    public:
        ConsoleGraphic();
        ~ConsoleGraphic(){ // prevent memory leak
            delete[] map;
        }
        void run();
};









ConsoleGraphic::ConsoleGraphic(){
    
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
    fontInfo.cbSize=sizeof(fontInfo);
    hout = GetStdHandle(STD_OUTPUT_HANDLE);
    buffCord = {0,0}; // DEFAULT
    buffWidth = 0;
    buffHeight = 0;
    createBuffering(7,150,100);
    
}

int ConsoleGraphic::onCreate(){
    return 0;
}

int ConsoleGraphic::onUpdate(){
    return 0;
}

std::string ConsoleGraphic::logger(){
    return "Time Elappsed: " + std::to_string(elappsedTime) + "\n";
}

void ConsoleGraphic::frameT(){
    while (runTest){
        
        auto start = std::chrono::system_clock::now();
        onUpdate();
        DWORD num;
        //FillConsoleOutputCharacterA(hout, 'a', 100, buffCord, &num);
        //WriteConsoleOutputCharacterA(hout,"dadfefdfefd",100,buffCord,&num);
        WriteConsoleOutput(hout,map,buffSize,buffCord,&writeArea);
        auto end = std::chrono::system_clock::now();
        auto elappsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        elappsedTime = elappsed.count();

        
        if (elappsedTime < minTimePerFrame){
            std::this_thread::sleep_for(std::chrono::milliseconds(minTimePerFrame - elappsedTime));
        }
    }
}

void ConsoleGraphic::createBuffering(int fontSize,short setWidth, short setHeight){
    if (fontSize != 0 && setWidth > 0 && setHeight > 0){
        //if (map != NULL){delete[] map;};

        COORD dwSize = {setWidth,setHeight};
        buffWidth = setWidth;
        buffHeight = setHeight;
        fontInfo.dwFontSize.X = fontSize;
        fontInfo.dwFontSize.Y = fontSize;
        
        SetCurrentConsoleFontEx(hout,true,&fontInfo);
        map = new CHAR_INFO[buffWidth*buffHeight];
        buffSize = {buffWidth,buffHeight};
        writeArea = {0,0,(short)(buffWidth-1),(short)(buffHeight-1)};
        
        cls();
        SetConsoleWindowInfo(hout,true,&writeArea);
        SetConsoleScreenBufferSize(hout,dwSize);
        SetConsoleWindowInfo(hout,true,&writeArea);
        SetConsoleScreenBufferSize(hout,dwSize);
    }
}

void ConsoleGraphic::writeV2(SHORT frame){
    if (map != NULL){
        SHORT temp = frame * 100;
        writeArea.Top = temp;
        //writeArea = {temp,10,(short)(width-1),(short)(height-1)};
        //buffSize = {200,20};
        //buffCord = {30,50};
        WriteConsoleOutput(hout,map,buffSize,buffCord,&writeArea);
    }
}








void ConsoleGraphic::cls(){
    for (int i = 0; i < buffWidth*buffHeight; i++){
        map[i].Char.AsciiChar = lightScale[0];
        map[i].Attributes = 0;
    }
}

void ConsoleGraphic::setFont(int x, int y,PIXEL_ATTR attr){
    if (x < buffWidth && x > 0 && y > 0 && y < buffHeight && attr.greyLevel < sizeof(lightScale)/sizeof(lightScale[0])){
        map[x+y*buffWidth].Char.AsciiChar = lightScale[attr.greyLevel];
        map[x+y*buffWidth].Attributes = attr.color;
    }
}

void ConsoleGraphic::drawLine(float x1, float y1, float x2, float y2,PIXEL_ATTR lineAttr){
    int tx,ty;
	float k = (y2 - y1) / (x2 - x1);
	float bx = y1 - k * x1; // || bx = y2 - k * x2;
	float by = x1 - y1/k;

    if (x1 < x2) { tx = x1;}else { tx = x2;}
	if (y1 < y2) { ty = y1;}else { ty = y2;}
    
    for (int i = 0; i < abs(x2-x1) || i < abs(y2-y1); i++){
        if (k >= -1 && k <=1) {
			tx += 1;
			ty = k * tx + bx;
		} else {
			ty += 1;
			tx = 1 / k * ty + by;
		}
		setFont(tx, ty,lineAttr);
    }
    setFont(x1, y1,lineAttr);
	setFont(x2, y2,lineAttr);

    /*
    float k = (y2 - y1)/(x2 - x1);
    for (float i = 0;i <= abs(x2-x1) || i <= abs(y2-y1); i+= 1){
        int x = x1 + i;
		int y = y1 + x*k;
        setFont(x,y, 4,WHITE);
	}
	setFont(x1, y1, 4,RED);
	setFont(x2, y2, 4,RED);*/
}



void ConsoleGraphic::drawRect(int x, int y, int width, int height,PIXEL_ATTR borderAttr){
    drawLine(x,y,x+width-1,y,borderAttr);
    drawLine(x,y,x,y+height-1,borderAttr);
    drawLine(x+width-1,y,x+width-1,y+height-1,borderAttr);
    drawLine(x,y+height-1,x+width-1,y+height-1,borderAttr);
}
void ConsoleGraphic::drawRect(int x, int y, int width, int height,PIXEL_ATTR borderAttr,PIXEL_ATTR fillAttr){
    for (int iy = y; iy < (y+height); iy++){
        for (int ix = x; ix < (x+width); ix++){
            setFont(ix,iy,fillAttr);
        }
    }
    drawLine(x,y,x+width-1,y,borderAttr);
    drawLine(x,y,x,y+height-1,borderAttr);
    drawLine(x+width-1,y,x+width-1,y+height-1,borderAttr);
    drawLine(x,y+height-1,x+width-1,y+height-1,borderAttr);
}

void ConsoleGraphic::run(){
    onCreate();
    std::thread th([=](){
        while (runTest){
            test.run(&logger()[0]);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
    std::thread th1(&ConsoleGraphic::frameT,this);
    th.join();
    th1.join();
    //th.detach();
}

#endif // CONSOLE_GRAPHIC_H