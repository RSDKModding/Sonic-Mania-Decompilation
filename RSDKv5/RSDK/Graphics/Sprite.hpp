#ifndef SPRITE_H
#define SPRITE_H

namespace RSDK
{

struct Image {
    Image()
    {
        InitFileInfo(&info);
        width   = 0;
        height  = 0;
        palette = NULL;
        pixels  = NULL;
    }

    virtual bool32 Load(const char *fileName, bool32 loadHeader) { return false; }
    virtual bool32 Load(String *fileName, bool32 loadHeader)
    {
        if (!fileName->chars)
            return Load("", loadHeader);

        GetCString(textBuffer, fileName);
        return Load(textBuffer, loadHeader);
    }
    virtual void Close() { CloseFile(&info); }

    FileInfo info;
    int32 width;
    int32 height;
    int32 depth;
    color *palette;
    uint8 *pixels;
};

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

struct ImageGIF : public Image {
    ImageGIF() { AllocateStorage(sizeof(GifDecoder), (void **)&decoder, DATASET_TMP, true); }

    bool32 Load(const char *fileName, bool32 loadHeader);

    GifDecoder *decoder;
};

#if RETRO_REV02
enum PNGColorFormats {
    PNGCLR_GREYSCALE  = 0,
    PNGCLR_RGB        = 2,
    PNGCLR_INDEXED    = 3,
    PNGCLR_GREYSCALEA = 4,
    PNGCLR_RGBA       = 6,
};

enum PNGCompressionFilters {
    PNGFILTER_NONE,
    PNGFILTER_SUB,
    PNGFILTER_UP,
    PNGFILTER_AVG,
    PNGFILTER_PAETH,
};

struct ImagePNG : public Image {
    bool32 Load(const char *fileName, bool32 loadHeader);

    void UnpackPixels_Greyscale(uint8 *pixelData);
    void UnpackPixels_GreyscaleA(uint8 *pixelData);
    void UnpackPixels_Indexed(uint8 *pixelData);
    void UnpackPixels_RGB(uint8 *pixelData);
    void UnpackPixels_RGBA(uint8 *pixelData);

    void Unfilter(uint8 *recon);

    bool32 AllocatePixels();
    void ProcessScanlines();

    uint8 bitDepth;
    uint8 colorFormat;
    uint8 compression;
    uint8 filter;
    uint8 interlaced;
    int32 chunkHeader;
    int32 chunkSize;
    int32 chunkCRC;
    int32 dataSize;
    uint8 *chunkBuffer;
};
#else
struct ImageTGA : public Image {
    bool32 Load(const char *fileName, bool32 loadHeader);
};
#endif

uint16 LoadSpriteSheet(const char *filename, int32 scope);
bool32 LoadImage(const char *filename, double displayTime, double delta, bool32 (*skipCallback)());

} // namespace RSDK

#endif // SPRITE_H