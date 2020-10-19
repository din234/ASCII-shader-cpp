
#include <iostream>
#include <string>

#include "include/bitmap.h"
#include "include/consoleGraphic.h"
//#include <assert.h>

class imageRenderer : public ConsoleGraphic{
    private:
        int imageWidth,imageHeight;
        PIXEL_ATTR *imagePix;

        float sensitivity = 20.00f / 1000.00f; // v(pixel/miliseccond) = s(pixel)/t(ms)
        const float zoomSensitivity = 100.00f; // higher = slower

        float zoom = 1.00f;
        float offSetX = 0.00f;
        float offSetY = 0.00f;
    private:
        void checkUserInput();
    private:
        int onCreate() override;
        int onUpdate() override;
        std::string logger() override;
    public:
        imageRenderer();
        ~imageRenderer(){
            delete[] imagePix;
        }

        //void test(){
            //processTest proc;
        //}
};


imageRenderer::imageRenderer(){}

void imageRenderer::checkUserInput(){
    int temp = elappsedTime;
    if (temp < minTimePerFrame){
        temp = minTimePerFrame;
    }
    //elappsedTime = 1; // testing

    if (GetAsyncKeyState(VK_LEFT) < 0){
        offSetX-= temp*sensitivity;
    } else if (GetAsyncKeyState(VK_RIGHT) < 0){
        offSetX+= temp*sensitivity;
    } else if (GetAsyncKeyState(VK_UP) < 0 ){
        offSetY-= temp*sensitivity;
    } else if (GetAsyncKeyState(VK_DOWN) < 0){
        offSetY+= temp*sensitivity;
    } else if (GetAsyncKeyState('X') < 0){
        zoom+= temp*sensitivity;
    } else if (GetAsyncKeyState('Z') < 0 && zoom > 1){
        zoom-= temp*sensitivity;
    } else if (GetAsyncKeyState(VK_ESCAPE) < 0){
        SetConsoleTitleA("Stopped!!!");
        std::cin.ignore();
    } else if (GetAsyncKeyState(VK_SPACE) < 0){
        runTest = false;
    }
}


std::string imageRenderer::logger(){
    return "Time Elappsed: " + std::to_string(elappsedTime) + "\n" + 
    "OFFSET X: "  + std::to_string(offSetX) + "\n" + 
    "OFFSET Y: "  + std::to_string(offSetY) + "\n" +
    "ZOOM OUT: "  + std::to_string(zoom) + "\n";
}

int imageRenderer::onCreate(){
    bitMap image("./image/24_bit/lena_color.bmp");
    int imageSize = image.getWidth()*image.getHeight();

    imageWidth = image.getWidth();
    imageHeight = image.getHeight();
    imagePix = new PIXEL_ATTR[imageSize];

    for (int i = 0; i < imageSize; i++){
        int atrValue = 15;
        int temp = (image.getPixel(i,0) + image.getPixel(i,1) + image.getPixel(i,2))/3;
        
        if (image.getPixel(i,0) < 128){
            atrValue -= FOREGROUND_RED;
        }
        if (image.getPixel(i,1) < 128){
            atrValue -= FOREGROUND_GREEN;
        }
        if (image.getPixel(i,2) < 128){
            atrValue -= FOREGROUND_BLUE;
        }
        if (temp < 128/2){
            atrValue -= FOREGROUND_INTENSITY;
        }
        imagePix[i].color = atrValue;
        imagePix[i].greyLevel = temp/52;
    }
    return 0;
}

int imageRenderer::onUpdate() {
    //test.run();
    checkUserInput();
    for (int x = 0; x < buffWidth; x++){
        for (int y = 0; y < buffHeight; y++){
            //if ((segment%flick + y) % flick != 0){
            int posX = x*zoom/zoomSensitivity + offSetX;
            int posY = y*zoom/zoomSensitivity + offSetY;
            if (posX > 0 && posY > 0 && posX < imageWidth && posY < imageHeight){
                int temp = posX + ((imageHeight -1) - posY) * imageWidth;
                setFont(x,y,imagePix[temp]);
            } else {
                setFont(x,y,{0,0});
            }
        }
    }
    return 0;
}