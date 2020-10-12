// #pragma once

#ifndef BITMAP_H
#define BITMAP_H

#include <Windows.h>

// Declaration
class bitMap {
    private:
        FILE* file;
        BITMAPFILEHEADER h{0};
        BITMAPINFOHEADER bitMapInfo{0};
        RGBTRIPLE *rgbt;
        
    public:
        bitMap(const char* fileName);
        ~bitMap(); // Destructor

        inline int getWidth();
        inline int getHeight();
        inline int getPixel(int i, int rgbtIndex);
};






// Definition
bitMap::bitMap(const char* fileName){
    file = fopen(fileName,"rb");

    if (file != NULL){
        size_t head = fread (&h,sizeof(BITMAPFILEHEADER),1,file);
        size_t info = fread (&bitMapInfo, sizeof(BITMAPINFOHEADER),1,file);

        // PIXEL DATA
        if (bitMapInfo.biClrUsed == 0){ // 24 bit and 32 bit image (alpha)
            int imageBandwidth = bitMapInfo.biWidth * bitMapInfo.biHeight;
            rgbt = new RGBTRIPLE[imageBandwidth];
                for (int i = 0; i < imageBandwidth; i++){
                    size_t pix = fread (&rgbt[i], sizeof(RGBTRIPLE) ,1,file);
                }
        }
        fclose(file);
    }
}

bitMap::~bitMap(){
    delete[] file;
    delete[] rgbt;
};

inline int bitMap::getWidth(){
    return bitMapInfo.biWidth;
}

inline int bitMap::getHeight(){
    return bitMapInfo.biHeight;
}




inline int bitMap::getPixel(int i,int rgbtIndex){
    if (i > 0 && i < bitMapInfo.biWidth * bitMapInfo.biHeight){
        
        switch (rgbtIndex) {
            case 0: return rgbt[i].rgbtRed;
            case 1: return rgbt[i].rgbtGreen;
            case 2: return rgbt[i].rgbtBlue;
        }
        /*
        int atrValue = 15;
        
        if (rgbt[temp].rgbtRed < 128){
            atrValue -= FOREGROUND_RED;
        }
        if (rgbt[temp].rgbtBlue < 128){
             atrValue -= FOREGROUND_BLUE;
        }
        if (rgbt[temp].rgbtGreen < 128){
            atrValue -= FOREGROUND_GREEN;
        }
        return atrValue;*/



    }
    return 0;
}



// Example (copy to main.cpp)
/*
int main(){
    bitMap image("./image/lena_color.bmp");
    HDC hdc = GetDC(GetConsoleWindow());
    while (true){
        system("cls");
        //int temp = 0;
        for (int x = 0; x < image.getWidth(); x++){
            for (int y = 0; y < image.getHeight(); ++y){
                int temp = x + y * image.getWidth();
                    SetPixel(hdc, x+10, image.getHeight() - y, RGB(image.rgbt[temp].rgbtRed,image.rgbt[temp].rgbtGreen,image.rgbt[temp].rgbtBlue));
                }
            }
            cin.ignore();
    }
    return 0;
}
*/



#endif // BITMAP_H