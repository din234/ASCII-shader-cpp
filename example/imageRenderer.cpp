
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
        int onInput(int keyCode) override;
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


imageRenderer::imageRenderer(){
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
}

int imageRenderer::onInput(int keyCode){
    switch (keyCode){
        case VK_ESCAPE: runTest = false;

        case VK_LEFT: offSetX-= elappsedTime*sensitivity;break;
        case VK_RIGHT: offSetX+= elappsedTime*sensitivity;break;
        case VK_UP: offSetY-= elappsedTime*sensitivity;break;
        case VK_DOWN: offSetY+= elappsedTime*sensitivity;break;
        case 'X': zoom+= elappsedTime*sensitivity;break;
        case 'Z': zoom-= elappsedTime*sensitivity;break;
    }
    return 0;
}


std::string imageRenderer::logger(){
    return "Time Elappsed: " + std::to_string(elappsedTime) + "\n" + 
    "OFFSET X: "  + std::to_string(offSetX) + "\n" + 
    "OFFSET Y: "  + std::to_string(offSetY) + "\n" +
    "ZOOM OUT: "  + std::to_string(zoom) + "\n";
}

int imageRenderer::onUpdate() {
    for (int x = 0; x < getBuffWidth(); x++){
        for (int y = 0; y < getBuffHeight(); y++){
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