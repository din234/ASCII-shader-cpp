// struct vs class: struct is public (can access everywhere by end-user) while class is private
#include <iostream>
#include <cstdarg>


#pragma pack(2) // https://stackoverflow.com/questions/3318410/pragma-pack-effect

struct bmpImageHeader { //BITMAPFILEHEADER
    // bmp header (14 byte) metadata
    unsigned short fileType; // 2 byte: file info (42 4d)
    unsigned int fileSize; // 4 byte: total byte
    unsigned short reserved1; // 2 byte: for more info
    unsigned short reserved2; // 2 byte: for more info
    unsigned int offset; // 4 byte ???
} header;

#pragma pack(2)

struct bmpImageDibHeader{ //BITMAPINFOHEADER
    // dib header (40 byte)
    unsigned int headerSize; // 4 byte: default 28 hex (40 decimal number of byte for BITMAPINFOHEADER)
    int imageWidth; // 4 byte
    int imageHeight; // 4 byte
    unsigned short imagePlanes; // 2 byte: number of color planes ???? (default = 1 in decimal)
    unsigned short imageBitCount; // 2 byte: number of bit per pixel
    unsigned int imageCompression; // 4 byte: default = 0 (no compression BL_RGB)
    unsigned int imageSize; // 4 byte: final size of compressed image => default = 0

    int xPixelPerMeter; // 4 byte: horizontal resolution of device; will be auto set (no prefer = 0)
    int yPixelPerMeter; // 4 byte: vertical resulution of device;
    
    unsigned int imageTotalColor; // 4 byte; if bit per pixel (bpp) > 8 => total color = 0 else = palete color
    unsigned int imageImpColor; // 4
} dibHeader;







/*
    https://kevinushey.github.io/blog/2016/01/27/introduction-to-c++-variadic-templates/

    Parameter pack

    unpack eg: 
    sum(t1,t2,t3) is recursive case = unpack
    sum(t1,t2,t3) => return t1 + sum(t2,t3) => sum(t2,t3) is recursive case = unpack
    sum(t2,t3) => return t2 + sum(t3) => sum(t3) is base case

    Expand function:
*/
template <typename T>
double sum(T t) { // Base: will be called if pack is empty (or contain 1 parameter)
    return t;
}

template <typename T, typename... Rest>
double sum(T t, Rest... rest) { // Recursive
    return t + sum(rest...); // unpack = rest...
}



template <typename T>
int mutiply(T t){
    return t*t;
}

template <typename T, typename... Rest>
int power_sum(T t,Rest... rest){
    return 
}


/*
    VariadicFunction
    variadicFunc("adfd",10,11,223,3);
*/
void variadicFunc(const char* count ...){
    va_list args;
    va_start(args,count);
    //va_start(&args,count);
    while (*count != '\0'){
        int i = va_arg(args,int);
        std::cout << i << std::endl;
        //std::cout << count << std::endl;;
        ++count;
    }
    va_end(args);
}

/*
    ENUMERATION (enum)
*/
enum shader {none,low,medium,high,bright}shade; // shade is an enum variable