
// #define _WIN32_WINNT 0x500 // window 2000 to later (= _WIN32_WINNT_WIN2K)

// header guard
#ifndef CONSOLE_GRAPHIC_H
#define CONSOLE_GRAPHIC_H


#include <Windows.h>
#include <string>
#include <math.h>
#include <thread>

#include "include/process.h"
#include "include/mathExtra.h"

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
typedef struct FCOORD {
    float x = 0x00f;
    float y = 0x00f;
}fcord;

typedef struct PIXEL_ATTR {
    int greyLevel = 0;
    int color = 0;
}pix;


class ConsoleGraphic {
    private:
        HANDLE hIn,hOut;
        INPUT_RECORD ckey;

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

        short buffWidth,buffHeight;
    protected:
        const int minTimePerFrame = 33; // ms
        int elappsedTime = 0;
        bool runTest = true;

        MOUSE_EVENT_RECORD mouses;

        std::vector<int> inputQueue;

    private: // GET CONSOLE INPUT BUFFER
        void getKeyEventToQueue(KEY_EVENT_RECORD key);
        void getMouseState(MOUSE_EVENT_RECORD mouse);
    
    private: // THREAD
        void inputThread();
        void logicThread();
    protected: // INHERITANCE
        void createBuffering(int fontSize,short setWidth, short setHeight);
        virtual int onCreate();
        virtual int onInput(int keyCode);
        virtual int onUpdate();
        virtual std::string logger();

    protected: // MODIFY OUTPUT
        inline short getBuffWidth();
        inline short getBuffHeight();

        void cls();
        void setFont(int x, int y,PIXEL_ATTR attr);
        void drawLine(float x1,float y1,float x2,float y2,PIXEL_ATTR lineAttr);

        void drawTri(FCOORD *cord,PIXEL_ATTR borderAttr);
        void drawTri(FCOORD *cord,PIXEL_ATTR borderAttr,PIXEL_ATTR fillAttr);

        void drawRect(int x, int y, int width, int height,PIXEL_ATTR borderAttr);
        void drawRect(int x, int y, int width, int height,PIXEL_ATTR borderAttr,PIXEL_ATTR fillAttr);

        void drawPoly();

    public:
        ConsoleGraphic();
        ~ConsoleGraphic(){ // prevent memory leak
            inputQueue.clear();
            delete[] map;
        }
        void run();
};























ConsoleGraphic::ConsoleGraphic(){
    HWND consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
    fontInfo.cbSize=sizeof(fontInfo);

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    hIn = GetStdHandle(STD_INPUT_HANDLE);
    //SetConsoleMode(hIn,ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT);

    buffCord = {0,0}; // DEFAULT
    buffWidth = 0;
    buffHeight = 0;
    createBuffering(5,120,120);   
}





void ConsoleGraphic::createBuffering(int fontSize,short setWidth, short setHeight){
    if (fontSize != 0 && setWidth > 0 && setHeight > 0){
        //if (map != NULL){delete[] map;};

        COORD dwSize = {setWidth,setHeight};
        buffWidth = setWidth;
        buffHeight = setHeight;
        fontInfo.dwFontSize.X = fontSize;
        fontInfo.dwFontSize.Y = fontSize;
        
        SetCurrentConsoleFontEx(hOut,true,&fontInfo);
        map = new CHAR_INFO[buffWidth*buffHeight];
        buffSize = {buffWidth,buffHeight};
        writeArea = {0,0,(short)(buffWidth-1),(short)(buffHeight-1)};
        
        cls();
        SetConsoleWindowInfo(hOut,true,&writeArea);
        SetConsoleScreenBufferSize(hOut,dwSize);
        SetConsoleWindowInfo(hOut,true,&writeArea);
        SetConsoleScreenBufferSize(hOut,dwSize);
    }
}

int ConsoleGraphic::onCreate(){
    return 0;
}

int ConsoleGraphic::onInput(int keyCode){
    return 0;
}

int ConsoleGraphic::onUpdate(){
    return 0;
}










void ConsoleGraphic::getKeyEventToQueue(KEY_EVENT_RECORD key){
    if (key.bKeyDown && mathExtra::checkElement(&inputQueue,(int)key.wVirtualKeyCode)){
        inputQueue.push_back(key.wVirtualKeyCode);
    } else if (key.bKeyDown == 0){
        mathExtra::deleteElement(&inputQueue,(int)key.wVirtualKeyCode);
    }
}
void ConsoleGraphic::inputThread(){
    while(runTest){
        DWORD read;
        ReadConsoleInput(hIn,&ckey,1,&read);
        switch(ckey.EventType){
            case KEY_EVENT: getKeyEventToQueue(ckey.Event.KeyEvent);break;
            case MOUSE_EVENT: mouses = ckey.Event.MouseEvent;break;
        }
    }
}


void ConsoleGraphic::logicThread(){
    while (runTest){
        auto start = std::chrono::system_clock::now();
        for (size_t i = 0; i < inputQueue.size();i++){
            onInput(inputQueue.at(i));
        }
        onUpdate();
        WriteConsoleOutput(hOut,map,buffSize,buffCord,&writeArea);
        auto end = std::chrono::system_clock::now();
        auto elappsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        elappsedTime = elappsed.count();

        if (elappsedTime < minTimePerFrame){
            std::this_thread::sleep_for(std::chrono::milliseconds(minTimePerFrame - elappsedTime));
            elappsedTime = minTimePerFrame;
        }
    }
}

std::string ConsoleGraphic::logger(){
    return "Time Elappsed: " + std::to_string(elappsedTime) + "\n";
}











inline short ConsoleGraphic::getBuffWidth(){
    return buffWidth;
}

inline short ConsoleGraphic::getBuffHeight(){
    return buffHeight;
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

    tx = (x1 < x2) ? x1 : x2;
    ty = (y1 < y2) ? y1 : y2;

    //tx = (tx < 0) ? 0 : tx;

    //if (abs(x2-x1) > buffWidth || abs(y2-y1) > buffHeight){return;}; // prevent infinity loop
    if (k >= -1 && k <=1) {
        for (int i = ((tx < 0)?0:tx); i < abs(x2-x1)+tx && i < buffWidth; i++){
		    ty = k * i + bx;
            setFont(i, ty,lineAttr);
        }
	} else {
        for (int j = ((ty < 0)?0:ty); j < abs(y2-y1)+ty && j < buffHeight; j++){
		    tx = 1 / k * j + by;
            setFont(tx, j,lineAttr);
        }
	}
    setFont(x1, y1,lineAttr);
	setFont(x2, y2,lineAttr);
}




void ConsoleGraphic::drawTri(FCOORD *cord,PIXEL_ATTR borderAttr){
    drawLine(cord[0].x,cord[0].y,cord[1].x,cord[1].y,borderAttr);
    drawLine(cord[0].x,cord[0].y,cord[2].x,cord[2].y,borderAttr);
    drawLine(cord[1].x,cord[1].y,cord[2].x,cord[2].y,borderAttr);
}

void ConsoleGraphic::drawTri(FCOORD cord[3],PIXEL_ATTR borderAttr,PIXEL_ATTR fillAttr){
    // k = (y2 - y1)/(x2 - x1)
    // f(x) = (x-x1)*k=y-y1 => y = kx - (k*x1 - y1)
    // x = 0y + 3;

    //int iMin = 0, iMid = 0, iMax = 0; // cause wrong calculation??? some time = 3;
    int iMin = 0,iMid = 1,iMax = 2;
    float maxX = cord[iMax].x, maY = cord[iMax].y;
    float minX = cord[iMin].x, miY = cord[iMin].y;
    for (int i = 0; i < 3; i++){
        if (maxX < cord[i].x){maxX = cord[i].x; iMax = i;}
    }
    for (int j = 0; j < 3; j++){
        if (minX > cord[j].x){minX = cord[j].x; iMin = j;}
    }
    iMid = 3 - iMax - iMin;

    float k = (cord[iMax].y - cord[iMin].y)/(cord[iMax].x - cord[iMin].x);
    float kMin = (cord[iMid].y - cord[iMin].y)/(cord[iMid].x - cord[iMin].x);
    float kMax = (cord[iMax].y - cord[iMid].y)/(cord[iMax].x - cord[iMid].x);

    float b = (k*cord[iMin].x - cord[iMin].y);
    float bMin = (kMin*cord[iMin].x - cord[iMin].y);
    float bMax = (kMax*cord[iMid].x-cord[iMid].y);
    
    float x = (minX < 0)? 0: minX;
    for (x; x < cord[iMid].x && x < buffWidth;x++){
        // std::cout << temp << std::endl; // test error
        //std::cout << iMid << std::endl; // test error
        int giaoY2 = k*x-b;
        int giaoY1 = kMin*x-bMin;
        drawLine(x,giaoY1,x,giaoY2,fillAttr);
    }
    for (x; x < maxX && x < buffWidth;x++){
        int giaoY2 = k*x-b;
        int giaoY3 = kMax*x-bMax;
        drawLine(x,giaoY3,x,giaoY2,fillAttr);
    }
    
    drawTri(cord,borderAttr);
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
    drawRect(x,y,width,height,borderAttr);
}

void ConsoleGraphic::run(){
    onCreate();

    
    
    std::thread th([=](){
        while (runTest){
            test.run(&logger()[0]); // data race ()???
            std::this_thread::sleep_for(std::chrono::milliseconds(33));
        }
    });
    std::thread th1(&ConsoleGraphic::inputThread,this);
    std::thread th2(&ConsoleGraphic::logicThread,this);

    th.join();
    th1.join();
    th2.join();
}

#endif // CONSOLE_GRAPHIC_H