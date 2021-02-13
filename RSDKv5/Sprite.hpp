#ifndef SPRITE_H
#define SPRITE_H

struct GifDecoder {
    int depth;
    int clearCode;
    int eofCode;
    int runningCode;
    int runningBits;
    int prevCode;
    int currentCode;
    int maxCodePlusOne;
    int stackPtr;
    int shiftState;
    int fileState;
    int position;
    int bufferSize;
    uint shiftData;
    uint pixelCount;
    byte buffer[256];
    byte stack[4096];
    byte suffix[4096];
    uint prefix[4096];
};

struct ImageGIF {
    FileInfo info;
    int width;
    int height;
    int field_4C;
    int field_4D;
    int field_4E;
    int field_4F;
    uint* palette;
    byte *dataPtr;
    GifDecoder *decoder;
};

struct ImagePNG {
    FileInfo info;
    int width;
    int height;
    int depth;
    int *palette;
    byte *dataPtr;
    byte bitDepth;
    byte clrType;
    byte compression;
    byte filter;
    byte interlaced;
    int chunkHeader;
    int chunkSize;
    int chunkCRC;
    int dataSize;
    byte *dataBuffer;
};

bool32 LoadGIF(ImageGIF *image, const char *fileName, bool32 loadHeader);
bool32 LoadPNG(ImagePNG *image, const char *fileName, bool32 loadHeader);

ushort LoadSpriteSheet(const char *filename, Scopes scope);
void LoadImage(const char *filename, double displayTime, double delta, int (*callback)(void));

#endif // SPRITE_H