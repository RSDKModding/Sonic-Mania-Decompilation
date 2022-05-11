#include "RSDK/Core/RetroEngine.hpp"

using namespace RSDK;

const int32 LOADING_IMAGE = 0;
const int32 LOAD_COMPLETE = 1;
const int32 LZ_MAX_CODE   = 4095;
const int32 LZ_BITS       = 12;
const int32 FIRST_CODE    = 4097;
const int32 NO_SUCH_CODE  = 4098;

int32 codeMasks[] = { 0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095 };

int32 ReadGifCode(RSDK::ImageGIF *image);
uint8 ReadGifByte(RSDK::ImageGIF *image);
uint8 TraceGifPrefix(uint32 *prefix, int32 code, int32 clearCode);

void InitGifDecoder(RSDK::ImageGIF *image)
{
    uint8 val                      = ReadInt8(&image->info);
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
    for (int32 i = 0; i <= LZ_MAX_CODE; ++i) image->decoder->prefix[i] = (uint8)NO_SUCH_CODE;
}
void ReadGifLine(RSDK::ImageGIF *image, uint8 *line, int32 length, int32 offset)
{
    int32 i         = 0;
    int32 stackPtr  = image->decoder->stackPtr;
    int32 eofCode   = image->decoder->eofCode;
    int32 clearCode = image->decoder->clearCode;
    int32 prevCode  = image->decoder->prevCode;
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
        int32 gifCode = ReadGifCode(image);
        if (gifCode == eofCode) {
            if (i != length - 1 | image->decoder->pixelCount != 0) {
                return;
            }
            i++;
        }
        else {
            if (gifCode == clearCode) {
                for (int32 j = 0; j <= LZ_MAX_CODE; j++) {
                    image->decoder->prefix[j] = NO_SUCH_CODE;
                }
                image->decoder->runningCode    = image->decoder->eofCode + 1;
                image->decoder->runningBits    = image->decoder->depth + 1;
                image->decoder->maxCodePlusOne = 1 << image->decoder->runningBits;
                prevCode                       = (image->decoder->prevCode = NO_SUCH_CODE);
            }
            else {
                if (gifCode < clearCode) {
                    line[offset++] = (uint8)gifCode;
                    i++;
                }
                else {
                    if (gifCode<0 | gifCode> LZ_MAX_CODE) {
                        return;
                    }
                    int32 code;
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
                    int32 c = 0;
                    while (c++ <= LZ_MAX_CODE && code > clearCode && code <= LZ_MAX_CODE) {
                        image->decoder->stack[stackPtr++] = image->decoder->suffix[code];
                        code                              = image->decoder->prefix[code];
                    }
                    if (c >= LZ_MAX_CODE | code > LZ_MAX_CODE) {
                        return;
                    }
                    image->decoder->stack[stackPtr++] = (uint8)code;
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

int32 ReadGifCode(RSDK::ImageGIF *image)
{
    while (image->decoder->shiftState < image->decoder->runningBits) {
        uint8 b = ReadGifByte(image);
        image->decoder->shiftData |= (uint32)((uint32)b << image->decoder->shiftState);
        image->decoder->shiftState += 8;
    }
    int32 result = (int32)((unsigned long)image->decoder->shiftData & (unsigned long)(codeMasks[image->decoder->runningBits]));
    image->decoder->shiftData >>= image->decoder->runningBits;
    image->decoder->shiftState -= image->decoder->runningBits;
    if (++image->decoder->runningCode > image->decoder->maxCodePlusOne && image->decoder->runningBits < LZ_BITS) {
        image->decoder->maxCodePlusOne <<= 1;
        image->decoder->runningBits++;
    }
    return result;
}

uint8 ReadGifByte(RSDK::ImageGIF *image)
{
    uint8 c = '\0';
    if (image->decoder->fileState == LOAD_COMPLETE)
        return c;

    uint8 b;
    if (image->decoder->position == image->decoder->bufferSize) {
        b                          = ReadInt8(&image->info);
        image->decoder->bufferSize = (int32)b;
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

uint8 TraceGifPrefix(uint32 *prefix, int32 code, int32 clearCode)
{
    int32 i = 0;
    while (code > clearCode && i++ <= LZ_MAX_CODE) code = prefix[code];

    return code;
}
void ReadGifPictureData(RSDK::ImageGIF *image, int32 width, int32 height, bool32 interlaced, uint8 *gfxData)
{
    int32 array[]  = { 0, 4, 2, 1 };
    int32 array2[] = { 8, 8, 4, 2 };
    InitGifDecoder(image);
    if (interlaced) {
        for (int32 i = 0; i < 4; ++i) {
            for (int32 j = array[i]; j < height; j += array2[i]) {
                ReadGifLine(image, gfxData, width, j * width);
            }
        }
        return;
    }
    for (int32 h = 0; h < height; ++h) ReadGifLine(image, gfxData, width, h * width);
}

bool32 RSDK::ImageGIF::Load(const char *fileName, bool32 loadHeader)
{
    if (!decoder)
        return false;

    if (fileName) {
        if (!LoadFile(&info, fileName, FMODE_RB))
            return false;
        Seek_Set(&info, 6);
        width  = ReadInt16(&info);
        height = ReadInt16(&info);
        if (loadHeader)
            return true;
    }

    int32 data = ReadInt8(&info);
    // int32 has_pallete  = (data & 0x80) >> 7;
    // int32 colors       = ((data & 0x70) >> 4) + 1;
    int32 palette_size = (data & 0x7) + 1;
    if (palette_size > 0)
        palette_size = 1 << palette_size;

    Seek_Cur(&info, 2);

    if (!palette)
        AllocateStorage(0x100 * sizeof(int32), (void **)&palette, DATASET_TMP, true);

    if (!pixels)
        AllocateStorage(width * height, (void **)&pixels, DATASET_TMP, false);

    if (palette && pixels) {
        uint8 clr[3];
        int32 c = 0;
        do {
            ReadBytes(&info, clr, 3);
            palette[c] = (clr[0] << 16) | (clr[1] << 8) | (clr[2] << 0);
            ++c;
        } while (c != palette_size);

        uint8 buf = ReadInt8(&info);
        while (buf != ',') buf = ReadInt8(&info); // gif image start identifier

        ReadInt16(&info);
        ReadInt16(&info);
        ReadInt16(&info);
        ReadInt16(&info);
        data              = ReadInt8(&info);
        bool32 interlaced = (data & 0x40) >> 6;
        if (data >> 7 == 1) {
            int32 c = 0x80;
            do {
                ++c;
                ReadBytes(&info, clr, 3);
                palette[c] = (clr[0] << 16) | (clr[1] << 8) | (clr[2] << 0);
            } while (c != 0x100);
        }

        ReadGifPictureData(this, width, height, interlaced, pixels);

        CloseFile(&info);
        return true;
    }
    return false;
}

#if RETRO_REV02
void PNGUnpackGreyscale(RSDK::ImagePNG *image, uint8 *pixelData)
{
    uint8 *pixels = image->pixels;
    for (int32 c = 0; c < image->width * image->height; ++c) {
        int32 brightness = *pixelData++;
        int32 alpha      = *pixelData++;

        uint32 color = 0;

        color = brightness << 16;
        pixelData++;
        color |= brightness << 8;
        pixelData++;
        color |= brightness << 0;
        pixelData++;
        color |= alpha << 24;
        pixelData++;

        *pixels++ = color;
    }
}

void PNGUnpackGreyscaleA(RSDK::ImagePNG *image, uint8 *pixelData)
{
    color *pixels = (color *)image->pixels;
    for (int32 c = 0; c < image->width * image->height; ++c) {
        int32 brightness = *pixelData++;

        uint32 color = 0;

        color = brightness << 16;
        pixelData++;
        color |= brightness << 8;
        pixelData++;
        color |= brightness << 0;
        pixelData++;
        color |= 0xFF << 24;
        pixelData++;

        *pixels++ = color;
    }
}

void PNGUnpackRGB(RSDK::ImagePNG *image, uint8 *pixelData)
{
    color *pixels = (color *)image->pixels;
    for (int32 c = 0; c < image->width * image->height; ++c) {
        uint32 color = 0;

        color = *pixelData << 16;
        pixelData++;
        color |= *pixelData << 8;
        pixelData++;
        color |= *pixelData << 0;
        pixelData++;
        color |= 0xFF << 24;
        pixelData++;

        *pixels++ = color;
    }
}

void PNGUnpackRGBA(RSDK::ImagePNG *image, uint8 *pixelData)
{
    color *pixels = (color *)image->pixels;
    for (int32 c = 0; c < image->width * image->height; ++c) {
        uint32 color = 0;

        color |= *pixelData << 16;
        pixelData++;
        color |= *pixelData << 8;
        pixelData++;
        color |= *pixelData << 0;
        pixelData++;
        color |= *pixelData << 24;
        pixelData++;

        *pixels++ = color;
    }
}

void DecodePNGData(RSDK::ImagePNG *image, uint8 *dataPtr)
{
    int32 colorSize = (image->bitDepth + 7) >> 3;
    switch (image->clrType) {
        case 2: colorSize *= 3; break;
        case 4: colorSize *= 2; break;
        case 6: colorSize *= 4; break;
    }

    int32 pitch       = colorSize * image->width;
    uint8 *dataBuffer = dataPtr + 1;

    if (*dataPtr == 1 || *dataPtr == 3) {
        for (int32 c = 0; c < colorSize; ++c) {
            *dataPtr++ = *dataBuffer++;
        }

        if (colorSize < pitch) {
            uint8 *buf = &dataPtr[-colorSize];
            for (int32 c = 0; c < pitch - colorSize; ++c) {
                *dataPtr++ = *buf++ + *dataBuffer++;
            }
        }
    }
    else if (*dataPtr == 4) {
        for (int32 c = 0; c < colorSize; ++c) {
            *dataPtr++ = *dataBuffer++;
        }

        if (colorSize < pitch) {
            uint8 *buf = &dataPtr[-colorSize];
            for (int32 c = 0; c < pitch - colorSize; ++c) {
                *dataPtr++ = *buf++ + *dataBuffer++;
            }
        }
    }
    else if (pitch > 0) {
        for (int32 c = 0; c < colorSize * image->width; ++c) {
            *dataPtr++ = *dataBuffer++;
        }
    }

    for (int32 h = 1; h < image->height; ++h) {
        int32 type = *dataBuffer++;
        switch (type) {
            case 1:
                for (int32 c = 0; c < colorSize; ++c) *dataPtr++ = *dataBuffer++;

                if (colorSize < pitch) {
                    uint8 *buf = &dataPtr[-colorSize];
                    for (int32 c = 0; c < pitch - colorSize; ++c) {
                        *dataPtr++ = *buf++ + *dataBuffer++;
                    }
                }
                break;
            case 2: {
                uint8 *buf = &dataPtr[-pitch];
                for (int32 c = 0; c < pitch; ++c) *dataPtr++ = *buf++ + *dataBuffer++;
                break;
            }
            case 3: {
                uint8 *buf = &dataPtr[-pitch];
                for (int32 c = 0; c < colorSize; ++c) *dataPtr++ = *dataBuffer++ + (*buf++ >> 1);

                if (colorSize < pitch) {
                    uint8 *buf  = &dataPtr[-colorSize];
                    int32 count = colorSize - pitch;

                    for (int32 c = 0; c < pitch - colorSize; ++c) {
                        *dataPtr++ = *dataBuffer++ + ((*(buf - 1) + (buf++)[count]) >> 1);
                    }
                }
                break;
            }
            case 4: {
                uint8 *buf = &dataPtr[-pitch];
                for (int32 c = 0; c < colorSize; ++c) *dataPtr++ = *buf++ + *dataBuffer++;

                if (colorSize < pitch) {
                    buf           = &dataPtr[-pitch];
                    int32 count   = pitch - colorSize;
                    int32 countv2 = pitch - (pitch + colorSize);

                    for (int32 c = 0; c < pitch - colorSize; ++c) {
                        uint8 val1 = buf[count];
                        uint8 val2 = *buf;
                        uint8 val3 = buf[countv2];
                        int32 dif  = val1 - val3 + val2;

                        int32 count1 = val3 - val2;
                        if (dif > val1)
                            count1 = val2 - val3;

                        int32 count2 = val1 - val3;
                        if (dif <= val2)
                            count2 = val3 - val1;

                        int32 count3 = val3 - dif;
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
                for (int32 c = 0; c < pitch; ++c) *dataPtr++ = *dataBuffer++;
                break;
            }
        }
    }
}

// PNG Chunk Header Signatures
#define PNG_SIG_HEADER  0x52444849 // IHDR
#define PNG_SIG_END     0x444E4549 // IEND
#define PNG_SIG_PALETTE 0x45544C50 // PLTE
#define PNG_SIG_DATA    0x54414449 // IDAT

bool32 RSDK::ImagePNG::Load(const char *fileName, bool32 loadHeader)
{
    if (fileName) {
        if (LoadFile(&info, fileName, FMODE_RB)) {
            if (ReadInt64(&info) == 0xA1A0A0D474E5089LL) {
                while (true) {
                    chunkSize   = ReadInt32(&info, true);
                    chunkHeader = ReadInt32(&info, false);

                    bool32 endFlag = false;
                    if (chunkHeader == PNG_SIG_HEADER && chunkSize == 13) {
                        width       = ReadInt32(&info, true);
                        height      = ReadInt32(&info, true);
                        bitDepth    = ReadInt8(&info);
                        clrType     = ReadInt8(&info);
                        compression = ReadInt8(&info);
                        filter      = ReadInt8(&info);
                        interlaced  = ReadInt8(&info);
                        if (interlaced || bitDepth != 8) {
                            CloseFile(&info);
                            return false;
                        }
                        depth = 32;
                        if (loadHeader)
                            return true;
                    }
                    else if (chunkHeader == PNG_SIG_END) {
                        endFlag = true;
                    }
                    else if (chunkHeader == PNG_SIG_PALETTE) {
                        int32 colorCnt = chunkSize / 3;
                        if (!(chunkSize % 3)) {
                            chunkSize = colorCnt;
                            if (colorCnt <= 0x100) {
                                if (!palette)
                                    AllocateStorage(sizeof(uint32) * colorCnt, (void **)&palette, DATASET_TMP, true);

                                uint8 clr[3];
                                for (int32 c = 0; c < colorCnt; ++c) {
                                    ReadBytes(&info, clr, 3 * sizeof(uint8));
                                    palette[c] = clr[2] + ((clr[1] + (clr[0] << 8)) << 8);
                                }
                            }
                        }
                    }
                    else if (chunkHeader == PNG_SIG_DATA) {
                        dataSize = sizeof(uint32) * height * (width + 1);
                        if (!pixels) {
                            AllocateStorage(dataSize, (void **)&pixels, DATASET_TMP, false);

                            if (!pixels) {
                                CloseFile(&info);
                                return false;
                            }
                        }
                        AllocateStorage(chunkSize, (void **)&chunkBuffer, DATASET_TMP, false);
                        ReadBytes(&info, chunkBuffer, chunkSize);

                        uint8 *pixelsPtr = NULL;
                        switch (clrType) {
                            case 0:
                            case 3: pixelsPtr = &this->pixels[width * height * 3]; break;

                            case 2: pixelsPtr = &this->pixels[width * height * 1]; break;

                            case 4: pixelsPtr = &this->pixels[width * height * 2]; break;

                            default: pixelsPtr = &this->pixels[0]; break;
                        }

                        ReadZLib(&info, (uint8 **)&chunkBuffer, chunkSize, (uint8 **)&this->pixels, dataSize);
                        DecodePNGData(this, pixelsPtr);

                        switch (clrType) {
                            case 0: PNGUnpackGreyscale(this, pixelsPtr); break;

                            case 2: PNGUnpackRGB(this, pixelsPtr); break;

                            case 3:
                                for (int32 c = 0; c < width * height; ++c) this->pixels[c] = palette[*this->pixels++] | 0xFF000000;
                                break;

                            case 4: PNGUnpackGreyscaleA(this, pixelsPtr); break;

                            case 6: PNGUnpackRGBA(this, pixelsPtr); break;

                            default: break;
                        }
                    }
                    else {
                        Seek_Cur(&info, chunkSize);
                    }

                    chunkCRC = ReadInt32(&info, false);

                    if (endFlag) {
                        CloseFile(&info);
                        return true;
                    }
                }
            }
            else {
                CloseFile(&info);
            }
        }
    }

    return false;
}
#endif

#if !RETRO_REV02
bool32 RSDK::ImageTGA::Load(const char *fileName, bool32 loadHeader)
{
    if (LoadFile(&info, fileName, FMODE_RB)) {
        uint8 startOffset     = ReadInt8(&info);
        uint8 colormaptype    = ReadInt8(&info);
        uint8 datatypecode    = ReadInt8(&info);
        int16 colormaporigin  = ReadInt16(&info);
        int16 colormaplength  = ReadInt16(&info);
        uint8 colormapdepth   = ReadInt8(&info);
        int16 originX         = ReadInt16(&info);
        int16 originY         = ReadInt16(&info);
        width                 = ReadInt16(&info);
        height                = ReadInt16(&info);
        uint8 imageBPP        = ReadInt8(&info);
        uint8 imagedescriptor = ReadInt8(&info);
        bool32 reverse        = (~imagedescriptor >> 4) & 1;
        if (imageBPP >= 0x10) {
            if (startOffset)
                Seek_Cur(&info, startOffset);

            AllocateStorage(sizeof(uint32) * height * width, (void **)&pixels, DATASET_TMP, false);
            uint32 *pixelsPtr = (uint32 *)pixels;
            if (reverse)
                pixelsPtr += (height * width) - width;

            int32 x = 0;
            if (datatypecode == 2) {
                switch (imageBPP) {
                    case 16:
                        for (int32 i = 0; i < height * width; ++i) {
                            uint8 bytes[2];
                            ReadBytes(&info, bytes, sizeof(uint16));
                            if (bytes[0] + (bytes[1] << 8) < 0)
                                *pixelsPtr++ = 8 * (bytes[0] & 0x1F | 8 * ((bytes[0] + (bytes[1] << 8)) & 0x3E0 | 0x3FC00));
                            else
                                *pixelsPtr++ = 0;

                            if (reverse && ++x == width) {
                                x = 0;
                                pixelsPtr -= width << 1;
                            }
                        }
                        break;

                    case 24:
                        for (int32 i = 0; i < height * width; ++i) {
                            uint8 channels[3];
                            ReadBytes(&info, channels, sizeof(color) - 1);
                            *pixelsPtr++ = (channels[0] << 0) | (channels[1] << 8) | (channels[2] << 16) | (0xFF << 24);

                            if (reverse && ++x == width) {
                                x = 0;
                                pixelsPtr -= width << 1;
                            }
                        }
                        break;

                    case 32:
                        for (int32 i = 0; i < height * width; ++i) {
                            uint8 channels[4];
                            ReadBytes(&info, channels, sizeof(color));
                            *pixelsPtr++ = (channels[0] << 0) | (channels[1] << 8) | (channels[2] << 16) | (channels[3] << 24);

                            if (reverse && ++x == width) {
                                x = 0;
                                pixelsPtr -= width << 1;
                            }
                        }
                        break;
                }
            }
            else if (datatypecode == 10) {
                switch (imageBPP) {
                    case 16: {
                        uint16 color16 = 0;
                        uint8 count = 0, flag = 0;
                        uint8 bytes[2];

                        for (int32 i = 0; i < height * width; ++i) {
                            if (count) {
                                if (!flag) {
                                    uint8 val[2];
                                    ReadBytes(&info, val, sizeof(uint16));
                                }
                                --count;
                            }
                            else {
                                uint8 flags = 0;
                                ReadBytes(&info, &flags, sizeof(uint8));
                                flag  = flags & 0x80;
                                count = flags & 0x7F;
                                flags &= 0x80;

                                ReadBytes(&info, bytes, sizeof(uint16));
                            }

                            color16 = bytes[0] + (bytes[1] << 8);
                            if (color16 < 0)
                                *pixelsPtr++ = 8 * (color16 & 0x1F | 8 * (color16 & 0x3E0 | 0x3FC00));
                            else
                                *pixelsPtr++ = 0;

                            ++pixelsPtr;
                            if (reverse && ++x == width) {
                                x = 0;
                                pixelsPtr -= width << 1;
                            }
                        }
                        break;
                    }

                    case 24: {
                        uint8 channels[3];
                        memset(channels, 0, sizeof(channels));
                        uint8 count = 0, flag = 0;
                        for (int32 i = 0; i < height * width; ++i) {
                            if (count) {
                                if (!flag) {
                                    ReadBytes(&info, channels, sizeof(color) - 1);
                                }
                                --count;
                            }
                            else {
                                uint8 flags = 0;
                                ReadBytes(&info, &flags, sizeof(uint8));
                                flag  = flags & 0x80;
                                count = flags & 0x7F;
                                flags &= 0x80;

                                ReadBytes(&info, channels, sizeof(color) - 1);
                            }
                            *pixelsPtr++ = (channels[0] << 0) | (channels[1] << 8) | (channels[2] << 16) | (0xFF << 24);

                            if (reverse && ++x == width) {
                                x = 0;
                                pixelsPtr -= width << 1;
                            }
                        }
                        break;
                    }

                    case 32: {
                        uint8 channels[sizeof(color)];
                        memset(channels, 0, sizeof(channels));
                        uint8 count = 0, flag = 0;

                        for (int32 i = 0; i < height * width; ++i) {
                            if (count) {
                                if (!flag) {
                                    ReadBytes(&info, channels, sizeof(uint32));
                                }
                                --count;
                            }
                            else {
                                uint8 flags = 0;
                                ReadBytes(&info, &flags, sizeof(uint8));
                                flag  = flags & 0x80;
                                count = flags & 0x7F;
                                flags &= 0x80;

                                ReadBytes(&info, channels, sizeof(color));
                            }
                            *pixelsPtr++ = (channels[0] << 0) | (channels[1] << 8) | (channels[2] << 16) | (channels[3] << 24);

                            if (reverse && ++x == width) {
                                x = 0;
                                pixelsPtr -= width << 1;
                            }
                        }
                        break;
                    }
                }
            }

            CloseFile(&info);
            return true;
        }
    }

    return false;
}
#endif

uint16 RSDK::LoadSpriteSheet(const char *filename, int32 scope)
{
    char fullFilePath[0x100];
    sprintf(fullFilePath, "Data/Sprites/%s", filename);

    RETRO_HASH_MD5(hash);
    GEN_HASH_MD5(filename, hash);

    for (int32 i = 0; i < SURFACE_MAX; ++i) {
        if (HASH_MATCH_MD5(gfxSurface[i].hash, hash)) {
            return i;
        }
    }

    uint16 id = -1;
    for (id = 0; id < SURFACE_MAX; ++id) {
        if (gfxSurface[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= SURFACE_MAX)
        return -1;

    GFXSurface *surface = &gfxSurface[id];
    ImageGIF image;

    if (image.Load(fullFilePath, true)) {
        surface->scope    = scope;
        surface->width    = image.width;
        surface->height   = image.height;
        surface->lineSize = 0;
        memcpy(surface->hash, hash, 4 * sizeof(int32));

        int32 w = surface->width;
        if (w > 1) {
            int32 ls = 0;
            do {
                w >>= 1;
                ++ls;
            } while (w > 1);
            surface->lineSize = ls;
        }

        surface->pixels = NULL;
        AllocateStorage(surface->width * surface->height, (void **)&surface->pixels, DATASET_STG, false);
        image.pixels = surface->pixels;
        image.Load(NULL, false);

        image.palette = NULL;
        image.decoder = NULL;
        CloseFile(&image.info);

        return id;
    }
    else {
        image.palette = NULL;
        image.pixels  = NULL;
        image.decoder = NULL;
        CloseFile(&image.info);
        return -1;
    }
}

bool32 RSDK::LoadImage(const char *filename, double displayLength, double speed, bool32 (*skipCallback)(void))
{
    char fullFilePath[0x100];
    sprintf(fullFilePath, "Data/Images/%s", filename);

#if RETRO_REV02
    ImagePNG image;
#else
    ImageTGA image;
#endif
    InitFileInfo(&image.info);

#if RETRO_REV02
    if (image.Load(fullFilePath, false)) {
        if (image.width == RETRO_VIDEO_TEXTURE_W && image.height == RETRO_VIDEO_TEXTURE_H) {
            RenderDevice::SetupImageTexture(image.width, image.height, image.pixels);
        }
#if !RETRO_USING_ORIGINAL_CODE
        else {
            PrintLog(PRINT_NORMAL, "ERROR: Images must be 1024x512!");
        }
#endif

        engine.displayTime              = displayLength;
        engine.storedShaderID           = RSDK::videoSettings.shaderID;
        engine.storedState              = sceneInfo.state;
        RSDK::videoSettings.dimMax      = 0.0;
        RSDK::videoSettings.shaderID    = SHADER_RGB_IMAGE;
        RSDK::videoSettings.screenCount = 0; // "Image Display Mode"
        engine.skipCallback             = skipCallback;
        sceneInfo.state                 = ENGINESTATE_SHOWIMAGE;
        engine.imageDelta               = speed / 60.0;

        image.palette = NULL;
        image.pixels  = NULL;
        CloseFile(&image.info);
        return true;
    }
#elif !RETRO_REV02
    if (image.Load(fullFilePath, true)) {
        if (image.width == RETRO_VIDEO_TEXTURE_W && image.height == RETRO_VIDEO_TEXTURE_H) {
            RenderDevice::SetupImageTexture(image.width, image.height, image.pixels);
        }
#if !RETRO_USING_ORIGINAL_CODE
        else {
            PrintLog(PRINT_NORMAL, "ERROR: Images must be 1024x512!");
        }
#endif

        engine.displayTime    = displayLength;
        engine.storedShaderID = engine.shaderID;
        engine.storedState    = sceneInfo.state;
        engine.dimMax         = 0.0;
        engine.shaderID       = SHADER_RGB_IMAGE;
        engine.screenCount    = 0; // "Image Display Mode"
        engine.skipCallback   = skipCallback;
        sceneInfo.state       = ENGINESTATE_SHOWIMAGE;
        engine.imageDelta     = speed / 60.0;

        image.palette = NULL;
        image.pixels  = NULL;
        CloseFile(&image.info);
        return true;
    }
#endif
    else {
        image.palette = NULL;
        image.pixels  = NULL;
        CloseFile(&image.info);
    }
    return false;
}
