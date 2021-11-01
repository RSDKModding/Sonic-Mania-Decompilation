#include "RetroEngine.hpp"

const int LOADING_IMAGE = 0;
const int LOAD_COMPLETE = 1;
const int LZ_MAX_CODE   = 4095;
const int LZ_BITS       = 12;
const int FIRST_CODE    = 4097;
const int NO_SUCH_CODE  = 4098;

int codeMasks[] = { 0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095 };

int ReadGifCode(ImageGIF *image);
byte ReadGifByte(ImageGIF *image);
byte TraceGifPrefix(uint *prefix, int code, int clearCode);

void InitGifDecoder(ImageGIF *image)
{
    byte val                       = ReadInt8(&image->info);
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
void ReadGifLine(ImageGIF *image, byte *line, int length, int offset)
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
                prevCode                       = (image->decoder->prevCode = NO_SUCH_CODE);
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
                        code                              = image->decoder->prefix[code];
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

int ReadGifCode(ImageGIF *image)
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

byte ReadGifByte(ImageGIF *image)
{
    byte c = '\0';
    if (image->decoder->fileState == LOAD_COMPLETE)
        return c;

    byte b;
    if (image->decoder->position == image->decoder->bufferSize) {
        b                          = ReadInt8(&image->info);
        image->decoder->bufferSize = (int)b;
        if (image->decoder->bufferSize == 0) {
            image->decoder->fileState = LOAD_COMPLETE;
            return c;
        }
        ReadBytes(&image->info, image->decoder->buffer, image->decoder->bufferSize);
        b                        = image->decoder->buffer[0];
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
void ReadGifPictureData(ImageGIF *image, int width, int height, bool32 interlaced, byte *gfxData)
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

bool32 LoadGIF(ImageGIF *image, const char *fileName, bool32 loadHeader)
{
    if (fileName) {
        if (!LoadFile(&image->info, fileName, FMODE_RB))
            return 0;
        Seek_Set(&image->info, 6);
        image->width  = ReadInt16(&image->info);
        image->height = ReadInt16(&image->info);
        if (loadHeader)
            return 1;
    }

    int data         = ReadInt8(&image->info);
    //int has_pallete  = (data & 0x80) >> 7;
    //int colors       = ((data & 0x70) >> 4) + 1;
    int palette_size = (data & 0x7) + 1;
    if (palette_size > 0)
        palette_size = 1 << palette_size;

    Seek_Cur(&image->info, 2);

    if (!image->palette)
        AllocateStorage(0x100 * sizeof(int), (void **)&image->palette, DATASET_TMP, true);

    if (!image->dataPtr)
        AllocateStorage(image->width * image->height, (void **)&image->dataPtr, DATASET_TMP, false);

    if (image->palette && image->dataPtr) {
        byte clr[3];
        int c = 0;
        do {
            ReadBytes(&image->info, clr, 3);
            image->palette[c] = (clr[0] << 16) | (clr[1] << 8) | (clr[2] << 0);
            ++c;
        } while (c != palette_size);

        byte buf = ReadInt8(&image->info);
        while (buf != ',') buf = ReadInt8(&image->info); // gif image start identifier

        ReadInt16(&image->info);
        ReadInt16(&image->info);
        ReadInt16(&image->info);
        ReadInt16(&image->info);
        data            = ReadInt8(&image->info);
        bool32 interlaced = (data & 0x40) >> 6;
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


#if RETRO_REV02
void PNGUnpackGreyscale(ImagePNG *image, byte *pixelData)
{
    byte *pixels = image->dataPtr;
    for (int c = 0; c < image->width * image->height; ++c) {
        int brightness = *pixelData++;
        int alpha = *pixelData++;

        uint colour = 0;

        colour = brightness << 16;
        pixelData++;
        colour |= brightness << 8;
        pixelData++;
        colour |= brightness << 0;
        pixelData++;
        colour |= alpha << 24;
        pixelData++;

        *pixels++ = colour;
    }
}

void PNGUnpackGreyscaleA(ImagePNG *image, byte *pixelData)
{
    colour *pixels = (colour *)image->dataPtr;
    for (int c = 0; c < image->width * image->height; ++c) {
        int brightness = *pixelData++;

        uint colour    = 0;

        colour = brightness << 16;
        pixelData++;
        colour |= brightness << 8;
        pixelData++;
        colour |= brightness << 0;
        pixelData++;
        colour |= 0xFF << 24;
        pixelData++;

        *pixels++ = colour;
    }
}

void PNGUnpackRGB(ImagePNG *image, byte *pixelData)
{
    colour *pixels = (colour *)image->dataPtr;
    for (int c = 0; c < image->width * image->height; ++c) {
        uint colour = 0;

        colour = *pixelData << 16;
        pixelData++;
        colour |= *pixelData << 8;
        pixelData++;
        colour |= *pixelData << 0;
        pixelData++;
        colour |= 0xFF << 24;
        pixelData++;

        *pixels++ = colour;
    }
}

void PNGUnpackRGBA(ImagePNG *image, byte *pixelData)
{
    colour *pixels = (colour*)image->dataPtr;
    for (int c = 0; c < image->width * image->height; ++c) {
        uint colour = 0;

        colour |= *pixelData << 16;
        pixelData++;
        colour |= *pixelData << 8;
        pixelData++;
        colour |= *pixelData << 0;
        pixelData++;
        colour |= *pixelData << 24;
        pixelData++;

        *pixels++ = colour;
    }
}

void DecodePNGData(ImagePNG *image, byte *dataPtr)
{
    int colourSize = (image->bitDepth + 7) >> 3;
    switch (image->clrType) {
        case 2: colourSize *= 3; break;
        case 4: colourSize *= 2; break;
        case 6: colourSize *= 4; break;
    }

    int pitch      = colourSize * image->width;
    uint8 *dataBuffer = dataPtr + 1;

    if (*dataPtr == 1 || *dataPtr == 3) {
        for (int c = 0; c < colourSize; ++c) {
            *dataPtr++ = *dataBuffer++;
        }

        if (colourSize < pitch) {
            uint8 *buf = &dataPtr[-colourSize];
            for (int c = 0; c < pitch - colourSize; ++c) {
                *dataPtr++ = *buf++ + *dataBuffer++;
            }
        }
    }
    else if (*dataPtr == 4) {
        for (int c = 0; c < colourSize; ++c) {
            *dataPtr++ = *dataBuffer++;
        }

        if (colourSize < pitch) {
            uint8 *buf = &dataPtr[-colourSize];
            for (int c = 0; c < pitch - colourSize; ++c) {
                *dataPtr++ = *buf++ + *dataBuffer++;
            }
        }
    }
    else if (pitch > 0) {
        for (int c = 0; c < colourSize * image->width; ++c) {
            *dataPtr++ = *dataBuffer++;
        }
    }

    for (int h = 1; h < image->height; ++h) {
        int type = *dataBuffer++;
        switch (type) {
            case 1:
                for (int c = 0; c < colourSize; ++c) *dataPtr++ = *dataBuffer++;

                if (colourSize < pitch) {
                    uint8 *buf = &dataPtr[-colourSize];
                    for (int c = 0; c < pitch - colourSize; ++c) {
                        *dataPtr++ = *buf++ + *dataBuffer++;
                    }
                }
                break;
            case 2: {
                uint8 *buf = &dataPtr[-pitch];
                for (int c = 0; c < pitch; ++c) *dataPtr++ = *buf++ + *dataBuffer++;
                break;
            }
            case 3: {
                uint8 *buf = &dataPtr[-pitch];
                for (int c = 0; c < colourSize; ++c) *dataPtr++ = *dataBuffer++ + (*buf++ >> 1);

                if (colourSize < pitch) {
                    uint8 *buf = &dataPtr[-colourSize];
                    int count = colourSize - pitch;

                    for (int c = 0; c < pitch - colourSize; ++c) {
                        *dataPtr++ = *dataBuffer++ + ((*(buf - 1) + (buf++)[count]) >> 1);
                    }
                }
                break;
            }
            case 4: {
                uint8 *buf = &dataPtr[-pitch];
                for (int c = 0; c < colourSize; ++c) *dataPtr++ = *buf++ + *dataBuffer++;

                if (colourSize < pitch) {
                    buf         = &dataPtr[-pitch];
                    int count   = pitch - colourSize;
                    int countv2 = pitch - (pitch + colourSize);

                    for (int c = 0; c < pitch - colourSize; ++c) {
                        uint8 val1   = buf[count];
                        uint8 val2   = *buf;
                        uint8 val3   = buf[countv2];
                        int dif    = val1 - val3 + val2;

                        int count1 = val3 - val2;
                        if (dif > val1)
                            count1 = val2 - val3;

                        int count2 = val1 - val3;
                        if (dif <= val2)
                            count2 = val3 - val1;

                        int count3 = val3 - dif;
                        if (dif > val3)
                            count3 = dif - val3;

                        if (count1 > count2 || count1 > count3) {
                            val1 = val3;
                            if (count2 <= count3)
                                val1 = *buf;
                        }

                        *dataPtr++ = val1 + *dataBuffer++;
                        ++buf;
                    }
                }
                break;
            }
            default: {
                for (int c = 0; c < pitch; ++c) *dataPtr++ = *dataBuffer++;
                break;
            }
        }
    }
}

bool32 LoadPNG(ImagePNG* image, const char* fileName, bool32 loadHeader) {
    if (fileName) {
        if (LoadFile(&image->info, fileName, FMODE_RB)) {
            if (ReadInt64(&image->info) == 0xA1A0A0D474E5089LL) {
                while (true) {
                    image->chunkSize   = ReadInt32(&image->info, true);
                    image->chunkHeader = ReadInt32(&image->info, false);

                    bool32 endFlag = false;
                    if (image->chunkHeader == 'RDHI' && image->chunkSize == 13) {
                        image->width       = ReadInt32(&image->info, true);
                        image->height      = ReadInt32(&image->info, true);
                        image->bitDepth    = ReadInt8(&image->info);
                        image->clrType     = ReadInt8(&image->info);
                        image->compression = ReadInt8(&image->info);
                        image->filter      = ReadInt8(&image->info);
                        image->interlaced  = ReadInt8(&image->info);
                        if (image->interlaced || image->bitDepth != 8) {
                            CloseFile(&image->info);
                            return false;
                        }
                        image->depth = 32;
                        if (loadHeader)
                            return true;
                    }
                    else if (image->chunkHeader == 'DNEI') {
                        endFlag = true;
                    }
                    else if (image->chunkHeader == 'ETLP') {
                        int colourCnt = image->chunkSize / 3;
                        if (!(image->chunkSize % 3)) {
                            image->chunkSize = colourCnt;
                            if (colourCnt <= 0x100) {
                                if (!image->palette)
                                    AllocateStorage(sizeof(uint) * colourCnt, (void **)&image->palette, DATASET_TMP, true);

                                byte clr[3];
                                for (int c = 0; c < colourCnt; ++c) {
                                    ReadBytes(&image->info, clr, 3 * sizeof(byte));
                                    image->palette[c] = clr[2] + ((clr[1] + (clr[0] << 8)) << 8);
                                }
                            }
                        }
                    }
                    else if (image->chunkHeader == 'TADI') {
                        image->dataSize = sizeof(uint) * image->height * (image->width + 1);
                        if (!image->dataPtr) {
                            AllocateStorage(image->dataSize, (void **)&image->dataPtr, DATASET_TMP, false);
                            if (!image->dataPtr) {
                                CloseFile(&image->info);
                                return false;
                            }
                        }
                        AllocateStorage(image->chunkSize, (void **)&image->chunkBuffer, DATASET_TMP, false);
                        ReadBytes(&image->info, image->chunkBuffer, image->chunkSize);

                        byte *dataPtr = NULL;
                        switch (image->clrType) {
                            case 0:
                            case 3: dataPtr = &image->dataPtr[3 * image->width * image->height]; break;
                            case 2: dataPtr = &image->dataPtr[image->width * image->height]; break;
                            case 4: dataPtr = &image->dataPtr[2 * image->width * image->height]; break;
                            default: dataPtr = image->dataPtr; break;
                        }

                        ReadZLib(&image->info, (byte **)&image->chunkBuffer, image->chunkSize, (byte **)&image->dataPtr, image->dataSize);
                        DecodePNGData(image, dataPtr);
                        switch (image->clrType) {
                            case 0: PNGUnpackGreyscale(image, dataPtr); break;
                            case 2: PNGUnpackRGB(image, dataPtr); break;
                            case 3:
                                for (int c = 0; c < image->width * image->height; ++c) {
                                    image->dataPtr[c] = image->palette[*dataPtr++] | 0xFF000000;
                                }
                                break;
                            case 4: PNGUnpackGreyscaleA(image, dataPtr); break;
                            case 6: PNGUnpackRGBA(image, dataPtr); break;
                            default: break;
                        }
                    }
                    else {
                        Seek_Cur(&image->info, image->chunkSize);
                    }
                    image->chunkCRC = ReadInt32(&image->info, false);

                    if (endFlag) {
                        CloseFile(&image->info);
                        return true;
                    }
                }
            }
            else {
                CloseFile(&image->info);
            }
        }
    }

    return false;
}
#endif

#if !RETRO_REV02
bool32 LoadTGA(ImagePNG *image, const char *filename)
{
    if (LoadFile(&image->info, filename, FMODE_RB)) {
        byte startOffset      = ReadInt8(&image->info);
        byte colourmaptype    = ReadInt8(&image->info);
        byte datatypecode     = ReadInt8(&image->info);
        short colourmaporigin = ReadInt16(&image->info);
        short colourmaplength = ReadInt16(&image->info);
        byte colourmapdepth   = ReadInt8(&image->info);
        short originX         = ReadInt16(&image->info);
        short originY         = ReadInt16(&image->info);
        image->xsize          = ReadInt16(&image->info);
        image->ysize         = ReadInt16(&image->info);
        byte imageBPP         = ReadInt8(&image->info);
        byte imagedescriptor  = ReadInt8(&image->info);
        bool32 reverse        = (~imagedescriptor >> 4) & 1;
        if (imageBPP >= 0x10) {
            if (startOffset)
                Seek_Cur(&image->info, startOffset);

            AllocateStorage(sizeof(uint) * image->ysize * image->xsize, (void **)&image->dataPtr, DATASET_TMP, false);
            uint *imageData = (uint *)image->dataPtr;
            if (reverse)
                imageData += (image->ysize * image->xsize) - image->xsize;

            int x = 0;
            if (datatypecode == 2) {
                switch (imageBPP) {
                    case 0x10:
                        for (int i = 0; i < image->ysize * image->xsize; ++i) {
                            byte bytes[2];
                            ReadBytes(&image->info, bytes, sizeof(ushort));
                            if (bytes[0] + (bytes[1] << 8) < 0)
                                *imageData++ = 8 * (bytes[0] & 0x1F | 8 * ((bytes[0] + (bytes[1] << 8)) & 0x3E0 | 0x3FC00));
                            else
                                *imageData++ = 0;

                            if (reverse && ++x == image->xsize) {
                                x = 0;
                                imageData -= image->xsize << 1;
                            }
                        }
                        break;
                    case 0x18:
                        for (int i = 0; i < image->ysize * image->xsize; ++i) {
                            byte channels[3];
                            ReadBytes(&image->info, channels, sizeof(colour) - 1);
                            *imageData++ = (channels[0] << 0) | (channels[1] << 8) | (channels[2] << 16) | (0xFF << 24);

                            if (reverse && ++x == image->xsize) {
                                x = 0;
                                imageData -= image->xsize << 1;
                            }
                        }
                        break;
                    case 0x20:
                        for (int i = 0; i < image->ysize * image->xsize; ++i) {
                            byte channels[4];
                            ReadBytes(&image->info, channels, sizeof(colour));
                            *imageData++ = (channels[0] << 0) | (channels[1] << 8) | (channels[2] << 16) | (channels[3] << 24);

                            if (reverse && ++x == image->xsize) {
                                x = 0;
                                imageData -= image->xsize << 1;
                            }
                        }
                        break;
                }
            }
            else if (datatypecode == 10) {
                switch (imageBPP) {
                    case 0x10: {
                        ushort colour16 = 0;
                        byte count = 0, flag = 0;
                        byte bytes[2];

                        for (int i = 0; i < image->ysize * image->xsize; ++i) {
                            if (count) {
                                if (!flag) {
                                    byte val[2];
                                    ReadBytes(&image->info, val, sizeof(ushort));
                                }
                                --count;
                            }
                            else {
                                byte flags = 0;
                                ReadBytes(&image->info, &flags, sizeof(byte));
                                flag  = flags & 0x80;
                                count = flags & 0x7F;
                                flags &= 0x80;

                                ReadBytes(&image->info, bytes, sizeof(ushort));
                            }

                            colour16 = bytes[0] + (bytes[1] << 8);
                            if (colour16 < 0)
                                *imageData++ = 8 * (colour16 & 0x1F | 8 * (colour16 & 0x3E0 | 0x3FC00));
                            else
                                *imageData++ = 0;

                            ++imageData;
                            if (reverse && ++x == image->xsize) {
                                x = 0;
                                imageData -= image->xsize << 1;
                            }
                        }
                        break;
                    }
                    case 0x18: {
                        byte channels[3];
                        memset(channels, 0, sizeof(channels));
                        byte count = 0, flag = 0;
                        for (int i = 0; i < image->ysize * image->xsize; ++i) {
                            if (count) {
                                if (!flag) {
                                    ReadBytes(&image->info, channels, sizeof(colour) - 1);
                                }
                                --count;
                            }
                            else {
                                byte flags = 0;
                                ReadBytes(&image->info, &flags, sizeof(byte));
                                flag  = flags & 0x80;
                                count = flags & 0x7F;
                                flags &= 0x80;

                                ReadBytes(&image->info, channels, sizeof(colour) - 1);
                            }
                            *imageData++ = (channels[0] << 0) | (channels[1] << 8) | (channels[2] << 16) | (0xFF << 24);

                            if (reverse && ++x == image->xsize) {
                                x = 0;
                                imageData -= image->xsize << 1;
                            }
                        }
                        break;
                    } 
                    case 0x20: {
                        byte channels[sizeof(colour)];
                        memset(channels, 0, sizeof(channels));
                        byte count = 0, flag = 0;
                        for (int i = 0; i < image->ysize * image->xsize; ++i) {
                            if (count) {
                                if (!flag) {
                                    ReadBytes(&image->info, channels, sizeof(uint));
                                }
                                --count;
                            }
                            else {
                                byte flags = 0;
                                ReadBytes(&image->info, &flags, sizeof(byte));
                                flag  = flags & 0x80;
                                count = flags & 0x7F;
                                flags &= 0x80;

                                ReadBytes(&image->info, channels, sizeof(colour));
                            }
                            *imageData++ = (channels[0] << 0) | (channels[1] << 8) | (channels[2] << 16) | (channels[3] << 24);

                            if (reverse && ++x == image->xsize) {
                                x = 0;
                                imageData -= image->xsize << 1;
                            }
                        }
                        break;
                    } 
                }
            }
            CloseFile(&image->info);
            return true;
        }
    }
    return false;
}
#endif

ushort LoadSpriteSheet(const char *filename, Scopes scope)
{
    char buffer[0x100];
    sprintf(buffer, "Data/Sprites/%s", filename);

    uint hash[4];
    GEN_HASH(filename, hash);

    for (int i = 0; i < SURFACE_MAX; ++i) {
        if (HASH_MATCH(gfxSurface[i].hash, hash)) {
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
    ImageGIF image;
    MEM_ZERO(image);
    InitFileInfo(&image.info);

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

        image.palette = NULL;
        image.decoder = NULL;
        CloseFile(&image.info);

        return id;
    }
    else {
        image.palette = NULL;
        image.dataPtr = NULL;
        image.decoder = NULL;
        CloseFile(&image.info);
        return -1;
    }
}

bool32 LoadImage(const char *filename, double displayLength, double speed, bool32 (*skipCallback)(void))
{
    char buffer[0x100];
    sprintf(buffer, "Data/Images/%s", filename);

    ImagePNG image;
    MEM_ZERO(image);
    InitFileInfo(&image.info);

#if RETRO_REV02
    if (LoadPNG(&image, buffer, false)) {
        if (image.width == 1024 && image.height == 512)
            SetImageTexture(image.width, image.height, image.dataPtr);

        engine.displayTime    = displayLength;
        engine.prevShaderID   = engine.shaderID;
        engine.prevEngineMode = sceneInfo.state;
        engine.dimMax         = 0.0;
        engine.shaderID       = SHADER_RGB_IMAGE;
        engine.screenCount    = 0;
        engine.skipCallback   = skipCallback;
        sceneInfo.state       = ENGINESTATE_SHOWPNG;
        engine.imageDelta     = speed / 60.0;

        image.palette = NULL;
        image.dataPtr = NULL;
        CloseFile(&image.info);
        return true;
    }
#elif !RETRO_REV02
    if (LoadTGA(&image, buffer)) {
        if (image.xsize == 1024 && image.ysize == 512)
            SetImageTexture(image.xsize, image.ysize, image.dataPtr);

        engine.displayTime    = displayLength;
        engine.prevShaderID   = engine.shaderID;
        engine.prevEngineMode = sceneInfo.state;
        engine.dimMax         = 0.0;
        engine.shaderID       = SHADER_RGB_IMAGE;
        engine.screenCount    = 0;
        engine.skipCallback   = skipCallback;
        sceneInfo.state       = ENGINESTATE_SHOWPNG;
        engine.imageDelta     = speed / 60.0;

        image.palette = NULL;
        image.dataPtr = NULL;
        CloseFile(&image.info);
        return true;
    }
#endif
    else {
        image.palette = NULL;
        image.dataPtr = NULL;
        CloseFile(&image.info);
    }
    return false;
}
