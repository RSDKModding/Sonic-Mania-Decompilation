#ifndef SPRITE_H
#define SPRITE_H

struct GifDecoder {
    int32 depth;
    int32 clearCode;
    int32 eofCode;
    int32 runningCode;
    int32 runningBits;
    int32 prevCode;
    int32 currentCode;
    int32 maxCodePlusOne;
    int32 stackPtr;
    int32 shiftState;
    int32 fileState;
    int32 position;
    int32 bufferSize;
    uint32 shiftData;
    uint32 pixelCount;
    uint8 buffer[256];
    uint8 stack[4096];
    uint8 suffix[4096];
    uint32 prefix[4096];
};

struct ImageGIF {
    FileInfo info;
    int32 width;
    int32 height;
    int32 unused;
    colour* palette;
    uint8 *dataPtr;
    GifDecoder *decoder;
};

#if RETRO_REV02
struct ImagePNG {
#else
struct ImageTGA {
#endif
    FileInfo info;
    int32 width;
    int32 height;
    int32 depth;
    colour *palette;
    uint8 *dataPtr;
    uint8 bitDepth;
    uint8 clrType;
    uint8 compression;
    uint8 filter;
    uint8 interlaced;
    int32 chunkHeader;
    int32 chunkSize;
    int32 chunkCRC;
    int32 dataSize;
    uint8 *chunkBuffer;
};

bool32 LoadGIF(ImageGIF *image, const char *fileName, bool32 loadHeader);
#if RETRO_REV02
bool32 LoadPNG(ImagePNG *image, const char *fileName, bool32 loadHeader);
#else
bool32 LoadTGA(ImageTGA *image, const char *fileName);
#endif

ushort LoadSpriteSheet(const char *filename, Scopes scope);
bool32 LoadImage(const char *filename, double displayTime, double delta, bool32 (*skipCallback)(void));

#endif // SPRITE_H