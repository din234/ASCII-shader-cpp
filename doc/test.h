// struct vs class: struct is public (can access everywhere by end-user) while class is private

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