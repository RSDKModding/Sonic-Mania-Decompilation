#include "RetroEngine.hpp"


const int LOADING_IMAGE = 0;
const int LOAD_COMPLETE = 1;
const int LZ_MAX_CODE   = 4095;
const int LZ_BITS       = 12;
const int FIRST_CODE    = 4097;
const int NO_SUCH_CODE  = 4098;

int codeMasks[] = { 0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095 };

int ReadGifCode(Image *image);
byte ReadGifByte(Image *image);
byte TraceGifPrefix(uint *prefix, int code, int clearCode);

void InitGifDecoder(Image *image)
{
    byte val                      = ReadInt8(&image->info);
    image->decoder->fileState      = LOADING_IMAGE;
    image->decoder->position       = 0;
    image->decoder->bufferSize     = 0;
    image->decoder->buffer[0]      = 0;
    image->decoder->depth          = val;
    image->decoder->clearCode      = 1 << val;
    image->decoder->eofCode        = image->decoder->clearCode + 1;
    image->decoder->runningCode    = image->decoder->eofCode + 1;
    image->decoder->runningBits    = val + 1;
    image->decoder->maxCodePlusOne = 1 << image->decoder->runningBits;
    image->decoder->stackPtr       = 0;
    image->decoder->prevCode       = NO_SUCH_CODE;
    image->decoder->shiftState     = 0;
    image->decoder->shiftData      = 0u;
    for (int i = 0; i <= LZ_MAX_CODE; ++i) image->decoder->prefix[i] = (byte)NO_SUCH_CODE;
}
void ReadGifLine(Image* image, byte *line, int length, int offset)
{
    int i         = 0;
    int stackPtr  = image->decoder->stackPtr;
    int eofCode   = image->decoder->eofCode;
    int clearCode = image->decoder->clearCode;
    int prevCode  = image->decoder->prevCode;
    if (stackPtr != 0) {
        while (stackPtr != 0) {
            if (i >= length) {
                break;
            }
            line[offset++] = image->decoder->stack[--stackPtr];
            i++;
        }
    }
    while (i < length) {
        int gifCode = ReadGifCode(image);
        if (gifCode == eofCode) {
            if (i != length - 1 | image->decoder->pixelCount != 0) {
                return;
            }
            i++;
        }
        else {
            if (gifCode == clearCode) {
                for (int j = 0; j <= LZ_MAX_CODE; j++) {
                    image->decoder->prefix[j] = NO_SUCH_CODE;
                }
                image->decoder->runningCode    = image->decoder->eofCode + 1;
                image->decoder->runningBits    = image->decoder->depth + 1;
                image->decoder->maxCodePlusOne = 1 << image->decoder->runningBits;
                prevCode                  = (image->decoder->prevCode = NO_SUCH_CODE);
            }
            else {
                if (gifCode < clearCode) {
                    line[offset++] = (byte)gifCode;
                    i++;
                }
                else {
                    if (gifCode<0 | gifCode> LZ_MAX_CODE) {
                        return;
                    }
                    int code;
                    if (image->decoder->prefix[gifCode] == NO_SUCH_CODE) {
                        if (gifCode != image->decoder->runningCode - 2) {
                            return;
                        }
                        code = prevCode;
                        image->decoder->suffix[image->decoder->runningCode - 2] =
                            (image->decoder->stack[stackPtr++] = TraceGifPrefix(image->decoder->prefix, prevCode, clearCode));
                    }
                    else {
                        code = gifCode;
                    }
                    int c = 0;
                    while (c++ <= LZ_MAX_CODE && code > clearCode && code <= LZ_MAX_CODE) {
                        image->decoder->stack[stackPtr++] = image->decoder->suffix[code];
                        code                         = image->decoder->prefix[code];
                    }
                    if (c >= LZ_MAX_CODE | code > LZ_MAX_CODE) {
                        return;
                    }
                    image->decoder->stack[stackPtr++] = (byte)code;
                    while (stackPtr != 0 && i++ < length) {
                        line[offset++] = image->decoder->stack[--stackPtr];
                    }
                }
                if (prevCode != NO_SUCH_CODE) {
                    if (image->decoder->runningCode<2 | image->decoder->runningCode> FIRST_CODE) {
                        return;
                    }
                    image->decoder->prefix[image->decoder->runningCode - 2] = prevCode;
                    if (gifCode == image->decoder->runningCode - 2) {
                        image->decoder->suffix[image->decoder->runningCode - 2] = TraceGifPrefix(image->decoder->prefix, prevCode, clearCode);
                    }
                    else {
                        image->decoder->suffix[image->decoder->runningCode - 2] = TraceGifPrefix(image->decoder->prefix, gifCode, clearCode);
                    }
                }
                prevCode = gifCode;
            }
        }
    }
    image->decoder->prevCode = prevCode;
    image->decoder->stackPtr = stackPtr;
}

int ReadGifCode(Image *image)
{
    while (image->decoder->shiftState < image->decoder->runningBits) {
        byte b = ReadGifByte(image);
        image->decoder->shiftData |= (uint)((uint)b << image->decoder->shiftState);
        image->decoder->shiftState += 8;
    }
    int result = (int)((unsigned long)image->decoder->shiftData & (unsigned long)(codeMasks[image->decoder->runningBits]));
    image->decoder->shiftData >>= image->decoder->runningBits;
    image->decoder->shiftState -= image->decoder->runningBits;
    if (++image->decoder->runningCode > image->decoder->maxCodePlusOne && image->decoder->runningBits < LZ_BITS) {
        image->decoder->maxCodePlusOne <<= 1;
        image->decoder->runningBits++;
    }
    return result;
}

byte ReadGifByte(Image *image)
{
    byte c = '\0';
    if (image->decoder->fileState == LOAD_COMPLETE)
        return c;

    byte b;
    if (image->decoder->position == image->decoder->bufferSize) {
        b                         = ReadInt8(&image->info);
        image->decoder->bufferSize = (int)b;
        if (image->decoder->bufferSize == 0) {
            image->decoder->fileState = LOAD_COMPLETE;
            return c;
        }
        ReadBytes(&image->info, image->decoder->buffer, image->decoder->bufferSize);
        b                   = image->decoder->buffer[0];
        image->decoder->position = 1;
    }
    else {
        b = image->decoder->buffer[image->decoder->position++];
    }
    return b;
}

byte TraceGifPrefix(uint *prefix, int code, int clearCode)
{
    int i = 0;
    while (code > clearCode && i++ <= LZ_MAX_CODE) code = prefix[code];

    return code;
}
void ReadGifPictureData(Image *image, int width, int height, bool interlaced, byte *gfxData)
{
    int array[]  = { 0, 4, 2, 1 };
    int array2[] = { 8, 8, 4, 2 };
    InitGifDecoder(image);
    if (interlaced) {
        for (int i = 0; i < 4; ++i) {
            for (int j = array[i]; j < height; j += array2[i]) {
                ReadGifLine(image, gfxData, width, j * width);
            }
        }
        return;
    }
    for (int h = 0; h < height; ++h) ReadGifLine(image, gfxData, width, h * width);
}

bool LoadGIF(Image* image, const char* fileName, bool dontLoadData) {
    if (fileName) {
        if (!LoadFile(&image->info, fileName))
            return 0;
        Seek_Set(&image->info, 6);
        image->width  = ReadInt16(&image->info);
        image->height = ReadInt16(&image->info);
        if (dontLoadData)
            return 1;
    }

    int data      = ReadInt8(&image->info);
    int has_pallete  = (data & 0x80) >> 7;
    int colors       = ((data & 0x70) >> 4) + 1;
    int palette_size = (data & 0x7) + 1;
    if (palette_size > 0)
        palette_size = 1 << palette_size;

    if (image->info.file) {
        if (image->info.usingFileBuffer)
            image->info.fileData += 2;
        else
            fSeek(image->info.file, 2, SEEK_CUR);

        if (image->info.encrypted)
            SkipBytes(&image->info, 2);
        image->info.readPos += 2;
    }

    if (!image->palette)
        AllocateStorage(0x100 * sizeof(int), (void **)&image->palette, DATASET_TMP, true);

    if (!image->dataPtr)
        AllocateStorage(image->width * image->height, (void **)&image->dataPtr, DATASET_TMP, false);

    if (image->palette && *image->dataPtr) {
        byte clr[3];
        int c = 0;
        do {
            ++c;
            ReadBytes(&image->info, clr, 3);
            image->palette[c] = (clr[0] << 16) | (clr[1] << 8) | (clr[2] << 0);
        } while (c != palette_size);

        byte buf = ReadInt8(&image->info);
        while (buf != ',') buf = ReadInt8(&image->info); // gif image start identifier

        ReadInt16(&image->info);
        ReadInt16(&image->info);
        ReadInt16(&image->info);
        ReadInt16(&image->info);
        data            = ReadInt8(&image->info);
        bool interlaced = (data & 0x40) >> 6;
        if (data >> 7 == 1) {
            int c = 0x80;
            do {
                ++c;
                ReadBytes(&image->info, clr, 3);
                image->palette[c] = (clr[0] << 16) | (clr[1] << 8) | (clr[2] << 0);
            } while (c != 0x100);
        }

        ReadGifPictureData(image, image->width, image->height, interlaced, image->dataPtr);

        CloseFile(&image->info);
        return true;
    }
    return false;
}

short LoadSpriteSheet(const char *filename, Scopes scope)
{
    char buffer[0x100];
    StrCopy(buffer, "Data/Sprites/");
    StrAdd(buffer, filename);

    uint hash[4];
    StrCopy(hashBuffer, filename);
    GenerateHash(hash, StrLength(filename));

    for (int i = 0; i < SURFACE_MAX; ++i) {
        if (memcmp(hash, gfxSurface[i].hash, 4 * sizeof(int)) == 0) {
            return i;
        }
    }

    ushort id = -1;
    for (id = 0; id < SURFACE_MAX; ++id) {
        if (gfxSurface[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= SURFACE_MAX)
        return -1;

    GFXSurface *surface = &gfxSurface[id];
    Image image;
    MEM_ZERO(image);

    AllocateStorage(sizeof(GifDecoder), (void **)&image.decoder, DATASET_TMP, true);

    if (LoadGIF(&image, buffer, true)) {
        surface->scope    = scope;
        surface->width    = image.width;
        surface->height   = image.height;
        surface->lineSize = 0;
        memcpy(surface->hash, hash, 4 * sizeof(int));

        int w = surface->width;
        if (w > 1) {
            int ls = 0;
            do {
                w >>= 1;
                ++ls;
            } while (w > 1);
            surface->lineSize = ls;
        }

        surface->dataPtr = NULL;
        AllocateStorage(surface->width * surface->height, (void **)&surface->dataPtr, DATASET_STG, false);
        image.dataPtr = surface->dataPtr;
        LoadGIF(&image, NULL, false);

        CloseFile(&image.info);

        return id;
    }
    else {
        CloseFile(&image.info);
        return -1;
    }
}