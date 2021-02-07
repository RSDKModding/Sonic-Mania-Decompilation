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

struct Image {
    FileInfo info;
    int width;
    int height;
    int field_4C;
    int field_4D;
    int field_4E;
    int field_4F;
    void* field_50;
    byte *dataPtr;
    GifDecoder *decoder;
};


bool LoadGIF(Image *image, const char *fileName, bool dontLoadData);

short LoadSpriteSheet(const char *filename, Scopes scope);

#endif // SPRITE_H