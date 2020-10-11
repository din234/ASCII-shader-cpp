#define _WIN32_WINNT 0x501

#include <iostream>
#include <cmath>

//#include <assert.h>

#include "include/bitmap.h"
#include "include/consoleGraphic.h"

struct stripe{
    int width;
    int height;
    int *greyScale;
    int *colorAttr;
}imageInfo;

class imageRenderer : public consoleGraphic{
    private:
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
    public:
        imageRenderer();
};


imageRenderer::imageRenderer(){}

void imageRenderer::checkUserInput(){
    if (elappsedTime < minTimePerFrame){
        elappsedTime = minTimePerFrame;
    }
    //elappsedTime = 1; // testing

    if (GetAsyncKeyState(VK_LEFT) < 0){
        offSetX-= elappsedTime*sensitivity;
    } else if (GetAsyncKeyState(VK_RIGHT) < 0){
        offSetX+= elappsedTime*sensitivity;
    } else if (GetAsyncKeyState(VK_UP) < 0 ){
        offSetY-= elappsedTime*sensitivity;
    } else if (GetAsyncKeyState(VK_DOWN) < 0){
        offSetY+= elappsedTime*sensitivity;
    } else if (GetAsyncKeyState('X') < 0){
        zoom+= elappsedTime*sensitivity;
    } else if (GetAsyncKeyState('Z') < 0 && zoom > 1){
        zoom-= elappsedTime*sensitivity;
    } else if (GetAsyncKeyState(VK_ESCAPE) < 0){
        SetConsoleTitleA("Stopped!!!");
        std::cin.ignore();
    }
}


int imageRenderer::onCreate(){
    bitMap image("./image/24_bit/lena_color.bmp");
    int imageSize = image.getWidth()*image.getHeight();

    imageInfo.width = image.getWidth();
    imageInfo.height = image.getHeight();
    imageInfo.greyScale = new int[imageSize];
    imageInfo.colorAttr = new int[imageSize];

    for (int i = 0; i < imageSize; i++){
        int atrValue = 15;
        imageInfo.greyScale[i] = (image.getPixel(i,0) + image.getPixel(i,1) + image.getPixel(i,2))/3;
        
        if (image.getPixel(i,0) < 128){
            atrValue -= FOREGROUND_RED;
        }
        if (image.getPixel(i,1) < 128){
             atrValue -= FOREGROUND_GREEN;
        }
        if (image.getPixel(i,2) < 128){
            atrValue -= FOREGROUND_BLUE;
        }
        if (imageInfo.greyScale[i] < 128/2){
            atrValue -= FOREGROUND_INTENSITY;
        }
        imageInfo.colorAttr[i] = atrValue;
    }
    return 0;
}

int imageRenderer::onUpdate() {
    checkUserInput();
    for (int x = 0; x < buffWidth; x++){
        for (int y = 0; y < buffHeight; y++){
            //if ((segment%flick + y) % flick != 0){
            int posX = x*zoom/zoomSensitivity + offSetX;
            int posY = y*zoom/zoomSensitivity + offSetY;
            if (posX > 0 && posY > 0 && posX < imageInfo.width && posY < imageInfo.height){
                int temp = posX + ((imageInfo.height -1) - posY) * imageInfo.width;
                setFont(x,y,imageInfo.greyScale[temp]/52,imageInfo.colorAttr[temp]);
            } else {
                setFont(x,y,0,0);
            }
        }
    }
    return 0;
}


int main(int argc ,char *argv[])
{
    imageRenderer imageRun;
    imageRun.run();
    return 0;
}



/*
        for (int x = 0; x < image.getWidth(); x++){
            for (int y = 0; y < image.getHeight(); ++y){
                int temp = x + y * image.getWidth();
                    SetPixel(hdc, x+10, image.getHeight() - y, RGB(image.rgbt[temp].rgbtRed,image.rgbt[temp].rgbtGreen,image.rgbt[temp].rgbtBlue));
                }
            }
            cin.ignore();
    }*/