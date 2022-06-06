#include "RSDK/Core/RetroEngine.hpp"

using namespace RSDK;

const int32 LOADING_IMAGE = 0;
const int32 LOAD_COMPLETE = 1;
const int32 LZ_MAX_CODE   = 4095;
const int32 LZ_BITS       = 12;
const int32 FIRST_CODE    = 4097;
const int32 NO_SUCH_CODE  = 4098;

int32 codeMasks[] = { 0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095 };

int32 ReadGifCode(ImageGIF *image);
uint8 ReadGifByte(ImageGIF *image);
uint8 TraceGifPrefix(uint32 *prefix, int32 code, int32 clearCode);

void InitGifDecoder(ImageGIF *image)
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
void ReadGifLine(ImageGIF *image, uint8 *line, int32 length, int32 offset)
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

int32 ReadGifCode(ImageGIF *image)
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

uint8 ReadGifByte(ImageGIF *image)
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
void ReadGifPictureData(ImageGIF *image, int32 width, int32 height, bool32 interlaced, uint8 *gfxData)
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

bool32 ImageGIF::Load(const char *fileName, bool32 loadHeader)
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

        Close();
        return true;
    }
    return false;
}

#if RETRO_REV02
void RSDK::ImagePNG::UnpackPixels_Greyscale(uint8 *pixelData)
{
    uint8 *pixels = this->pixels;
    for (int32 p = 0; p < this->width * this->height; ++p) {
        uint8 brightness = *pixelData;
        pixelData++;

        uint32 color = 0;

        // red channel
        color = brightness << 16;

        // green channel
        color |= brightness << 8;

        // blue channel
        color |= brightness << 0;

        // alpha channel
        color |= brightness << 24;

        *pixels = color;

        pixels += 2;
    }
}

void RSDK::ImagePNG::UnpackPixels_GreyscaleA(uint8 *pixelData)
{
    color *pixels = (color *)this->pixels;
    for (int32 p = 0; p < this->width * this->height; ++p) {
        uint8 brightness = *pixelData;
        pixelData++;

        uint32 color = 0;

        // red channel
        color = brightness << 16;

        // green channel
        color |= brightness << 8;

        // blue channel
        color |= brightness << 0;

        // alpha channel
        color |= 0xFF << 24;

        *pixels = color;

        pixels++;
    }
}

void RSDK::ImagePNG::UnpackPixels_Indexed(uint8 *pixelData)
{
    color *pixels = (color *)this->pixels;
    for (int32 p = 0; p < this->width * this->height; ++p) {
        pixels[p] = palette[pixelData[p]] | 0xFF000000;
    }
}

void RSDK::ImagePNG::UnpackPixels_RGB(uint8 *pixelData)
{
    color *pixels = (color *)this->pixels;
    for (int32 p = 0; p < this->width * this->height; ++p) {
        uint32 color = 0;

        // R
        color = *pixelData << 16;
        pixelData++;

        // G
        color |= *pixelData << 8;
        pixelData++;

        // B
        color |= *pixelData << 0;
        pixelData++;

        // A
        color |= 0xFF << 24;

        *pixels++ = color;
    }
}

void RSDK::ImagePNG::UnpackPixels_RGBA(uint8 *pixelData)
{
    color *pixels = (color *)this->pixels;
    for (int32 p = 0; p < this->width * this->height; ++p) {
        uint32 color = 0;

        // R
        color |= *pixelData << 16;
        pixelData++;

        // G
        color |= *pixelData << 8;
        pixelData++;

        // B
        color |= *pixelData << 0;
        pixelData++;

        // A
        color |= *pixelData << 24;
        pixelData++;

        *pixels++ = color;
    }
}

// from: https://raw.githubusercontent.com/lvandeve/lodepng/master/lodepng.cpp - paethPredictor()
uint8 paethPredictor(int16 a, int16 b, int16 c)
{
    int16 pa = abs(b - c);
    int16 pb = abs(a - c);
    int16 pc = abs(a + b - c - c);
    /* return input value associated with smallest of pa, pb, pc (with certain priority if equal) */
    if (pb < pa) {
        a  = b;
        pa = pb;
    }

    return (pc < pa) ? c : a;
}

void RSDK::ImagePNG::Unfilter(uint8 *recon)
{
    int32 bpp = (this->bitDepth + 7) >> 3;
    switch (this->colorFormat) {
        default: break;

        case PNGCLR_RGB: bpp *= sizeof(color) - 1; break;

        case PNGCLR_GREYSCALEA: bpp *= 2 * sizeof(uint8); break;

        case PNGCLR_RGBA: bpp *= sizeof(color); break;
    }

    int32 pitch     = bpp * this->width;
    uint8 *scanline = recon;

    for (int32 y = 0; y < this->height; ++y) {
        int32 filter = *scanline++;

        // prev scanline
        uint8 *precon = y ? &recon[-pitch] : NULL;

        switch (filter) {
            default:
#if !RETRO_USE_ORIGINAL_CODE
                PrintLog(PRINT_NORMAL, "Invalid PNG Filter: %d", filter);
                return;
#else
                // [Fallthrough]
#endif
            case PNGFILTER_NONE:
                for (int32 c = 0; c < pitch; ++c) {
                    recon[c] = scanline[c];
                }
                break;

            case PNGFILTER_SUB:
                for (int32 c = 0; c < bpp; ++c) {
                    recon[c] = scanline[c];
                }

                for (int32 c = bpp, p = 0; c < pitch; ++c, ++p) {
                    recon[c] = scanline[c] + recon[p];
                }
                break;

            case PNGFILTER_UP:
                if (precon) {
                    for (int32 c = 0; c < pitch; ++c) {
                        recon[c] = precon[c] + scanline[c];
                    }
                }
                else {
                    for (int32 c = 0; c < pitch; ++c) {
                        recon[c] = scanline[c];
                    }
                }
                break;

            case PNGFILTER_AVG:
                if (precon) {
                    for (int32 c = 0; c < bpp; ++c) {
                        recon[c] = scanline[c] + (precon[c] >> 1);
                    }

                    for (int32 c = bpp, p = 0; c < pitch; ++c, ++p) {
                        recon[c] = scanline[c] + ((recon[p] + precon[c]) >> 1);
                    }
                }
                else {
                    for (int32 c = 0; c < bpp; ++c) {
                        recon[c] = scanline[c];
                    }

                    for (int32 c = bpp, p = 0; c < pitch; ++c, ++p) {
                        recon[c] = scanline[c] + (recon[p] >> 1);
                    }
                }
                break;

            case PNGFILTER_PAETH:
                if (precon) {
                    for (int32 c = 0; c < bpp; ++c) {
                        recon[c] = (scanline[c] + precon[c]);
                    }

                    for (int32 c = bpp, p = 0; c < pitch; ++c, ++p) {
                        recon[c] = (scanline[c] + paethPredictor(recon[c - bpp], precon[c], precon[p]));
                    }
                }
                else {
                    for (int32 c = 0; c < bpp; ++c) {
                        recon[c] = scanline[c];
                    }

                    for (int32 c = bpp, p = 0; c < pitch; ++c, ++p) {
                        recon[c] = scanline[c] + recon[p];
                    }
                }
                break;
        }

        recon += pitch;
        scanline += pitch;
    }
}

bool32 RSDK::ImagePNG::AllocatePixels()
{
    dataSize = sizeof(color) * height * (width + 1);
    if (!pixels) {
        AllocateStorage(dataSize, (void **)&pixels, DATASET_TMP, false);

        if (!pixels) {
            Close();
            return false;
        }
    }

    return true;
}

void RSDK::ImagePNG::ProcessScanlines()
{
    uint8 *pixelsPtr = NULL;
    switch (colorFormat) {
        case PNGCLR_GREYSCALE:
        case PNGCLR_INDEXED: pixelsPtr = &pixels[3 * width * height]; break;

        case PNGCLR_RGB: pixelsPtr = &pixels[1 * width * height]; break;

        case PNGCLR_GREYSCALEA: pixelsPtr = &pixels[2 * width * height]; break;

        case PNGCLR_RGBA:
        default: pixelsPtr = &pixels[0 * width * height]; break;
    }

    // the original v5 decodes IDAT sections one at a time, so the compressed buffer is extracted into pixels every time here
    // again, this is a BAD idea!! IDAT chunks should be stored and processed as a group at the end of reading

    // decode all loaded IDAT chunks into pixels
    ReadZLib(&info, (uint8 **)&chunkBuffer, chunkSize, (uint8 **)&pixelsPtr, dataSize);

    Unfilter(pixelsPtr);

    switch (colorFormat) {
        case PNGCLR_GREYSCALE: UnpackPixels_Greyscale(pixelsPtr); break;

        case PNGCLR_RGB: UnpackPixels_RGB(pixelsPtr); break;

        case PNGCLR_INDEXED: UnpackPixels_Indexed(pixelsPtr); break;

        case PNGCLR_GREYSCALEA: UnpackPixels_GreyscaleA(pixelsPtr); break;

        case PNGCLR_RGBA: UnpackPixels_RGBA(pixelsPtr); break;

        default: break;
    }
}

// PNG format signature
#define PNG_SIGNATURE 0xA1A0A0D474E5089LL // PNG (and other bytes I don't care about)

// PNG chunk header signatures
#define PNG_SIG_HEADER  0x52444849 // IHDR
#define PNG_SIG_END     0x444E4549 // IEND
#define PNG_SIG_PALETTE 0x45544C50 // PLTE
#define PNG_SIG_DATA    0x54414449 // IDAT

bool32 RSDK::ImagePNG::Load(const char *fileName, bool32 loadHeader)
{
    if (fileName) {
        if (LoadFile(&info, fileName, FMODE_RB)) {
            if (ReadInt64(&info) == PNG_SIGNATURE) {
                while (true) {
                    chunkSize   = ReadInt32(&info, true);
                    chunkHeader = ReadInt32(&info, false);

                    bool32 finished = false;
                    if (chunkHeader == PNG_SIG_HEADER && chunkSize == 13) {
                        width       = ReadInt32(&info, true);
                        height      = ReadInt32(&info, true);
                        bitDepth    = ReadInt8(&info);
                        colorFormat = ReadInt8(&info);
                        compression = ReadInt8(&info);
                        filter      = ReadInt8(&info);
                        interlaced  = ReadInt8(&info);
                        if (interlaced || bitDepth != 8) {
                            Close();
                            return false;
                        }
                        depth = 32;

#if !RETRO_USE_ORIGINAL_CODE
                        // image size should be enough space to hold all the IDAT chunks
                        AllocateStorage(sizeof(color) * height * (width + 1), (void **)&chunkBuffer, DATASET_TMP, true);
                        dataSize = 0;
#endif

                        if (loadHeader)
                            return true;
                    }
                    else if (chunkHeader == PNG_SIG_END) {
                        finished = true;
                    }
                    else if (chunkHeader == PNG_SIG_PALETTE) {
                        int32 colorCnt = chunkSize / 3;
                        if (!(chunkSize % 3)) {
                            chunkSize = colorCnt;
                            if (colorCnt <= 0x100) {
                                if (!palette)
                                    AllocateStorage(sizeof(uint32) * colorCnt, (void **)&palette, DATASET_TMP, true);

                                uint8 channels[3];
                                for (int32 c = 0; c < colorCnt; ++c) {
                                    ReadBytes(&info, channels, 3 * sizeof(uint8));
                                    palette[c] = (channels[0] << 16) | (channels[1] << 8) | (channels[2] << 0);
                                }
                            }
                        }
                    }
                    else if (chunkHeader == PNG_SIG_DATA) {
#if RETRO_USE_ORIGINAL_CODE
                        if (!AllocatePixels())
                            return false;

                        // read this chunk into the chunk buffer storage (we're processing each IDAT section by itself
                        // this is a BAD idea!!! though it's kept here for reference as to how the original v5 works
                        AllocateStorage(chunkSize, (void **)&chunkBuffer, DATASET_TMP, false);
                        ReadBytes(&info, chunkBuffer, chunkSize);

                        // decode the scanlines into usable RGBA pixels
                        ProcessScanlines();
#else
                        // read this chunk into the chunk buffer storage (we process em all at the end)
                        dataSize += ReadBytes(&info, chunkBuffer + dataSize, chunkSize);
#endif
                    }
                    else {
                        Seek_Cur(&info, chunkSize);
                    }

                    chunkCRC = ReadInt32(&info, false);

                    if (finished) {
                        Close();

#if !RETRO_USE_ORIGINAL_CODE
                        // copy this over, since we only "borrowed" it after all :)
                        // chunkSize is the size of chunkBuffer
                        chunkSize = dataSize;
                        if (!AllocatePixels())
                            return false;

                        // decode the scanlines into usable RGBA pixels
                        ProcessScanlines();

                        RemoveStorageEntry((void **)&chunkBuffer);
#endif

                        return true;
                    }
                }
            }
            else {
                Close();
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
        // header
        uint8 idLength = ReadInt8(&info);

        // color map type
        uint8 colormaptype = ReadInt8(&info);

        // image type
        uint8 datatypecode = ReadInt8(&info);

        // color map specification
        int16 colormaporigin = ReadInt16(&info);
        int16 colormaplength = ReadInt16(&info);
        uint8 colormapdepth  = ReadInt8(&info);

        // image specification
        int16 originX    = ReadInt16(&info);
        int16 originY    = ReadInt16(&info);
        width            = ReadInt16(&info);
        height           = ReadInt16(&info);
        uint8 bpp        = ReadInt8(&info);
        uint8 descriptor = ReadInt8(&info);

        bool32 reverse = (~descriptor >> 4) & 1;
        if (bpp >= 16) {
            if (idLength)
                Seek_Cur(&info, idLength);

            AllocateStorage(sizeof(uint32) * height * width, (void **)&pixels, DATASET_TMP, false);
            uint32 *pixelsPtr = (uint32 *)pixels;
            if (reverse)
                pixelsPtr += (height * width) - width;

            int32 x = 0;
            switch (datatypecode) {
                case 2: // Uncompressed, RGB images
                    switch (bpp) {
                        case 16:
                            for (int32 i = 0; i < height * width; ++i) {
                                uint8 channels[2];
                                ReadBytes(&info, channels, sizeof(uint16));

                                uint16 color16 = channels[0] + (channels[1] << 8);
                                *pixelsPtr     = 0;

                                if (color16 & 0x8000) { // alpha bit (0 = invisible, 1 = visible)
                                    uint32 R = (color16 >> 10) & 0x1F;
                                    uint32 G = (color16 >> 5) & 0x1F;
                                    uint32 B = (color16 >> 0) & 0x1F;

                                    R = (R << 3) | (R >> 2);
                                    G = (G << 3) | (G >> 2);
                                    B = (B << 3) | (B >> 2);

                                    *pixelsPtr = (R << 16) | (G << 8) | (B << 0);
                                }

                                pixelsPtr++;

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

                                *pixelsPtr = (channels[0] << 0) | (channels[1] << 8) | (channels[2] << 16) | (0xFF << 24);
                                pixelsPtr++;

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

                                *pixelsPtr = (channels[0] << 0) | (channels[1] << 8) | (channels[2] << 16) | (channels[3] << 24);
                                pixelsPtr++;

                                if (reverse && ++x == width) {
                                    x = 0;
                                    pixelsPtr -= width << 1;
                                }
                            }
                            break;
                    }
                    break;

                case 10: // Runlength encoded RGB images
                    switch (bpp) {
                        case 16: {
                            uint8 channels[2];
                            memset(channels, 0, sizeof(channels));

                            uint8 count        = 0;
                            bool32 decodingRLE = false;
                            for (int32 p = 0; p < height * width; ++p) {
                                if (count) {
                                    if (!decodingRLE)
                                        ReadBytes(&info, channels, sizeof(uint16));

                                    --count;
                                }
                                else {
                                    uint8 count = ReadInt8(&info);
                                    decodingRLE = count & 0x80;
                                    count &= 0x7F;

                                    ReadBytes(&info, channels, sizeof(uint16));
                                }

                                uint16 color16 = channels[0] + (channels[1] << 8);
                                *pixelsPtr     = 0;

                                if (color16 & 0x8000) { // alpha bit (0 = invisible, 1 = visible)
                                    uint32 R = (color16 >> 10) & 0x1F;
                                    uint32 G = (color16 >> 5) & 0x1F;
                                    uint32 B = (color16 >> 0) & 0x1F;

                                    R = (R << 3) | (R >> 2);
                                    G = (G << 3) | (G >> 2);
                                    B = (B << 3) | (B >> 2);

                                    *pixelsPtr = (R << 16) | (G << 8) | (B << 0);
                                }

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

                            uint8 count        = 0;
                            bool32 decodingRLE = false;
                            for (int32 p = 0; p < height * width; ++p) {
                                if (count) {
                                    if (!decodingRLE)
                                        ReadBytes(&info, channels, sizeof(color) - 1);

                                    --count;
                                }
                                else {
                                    uint8 count = ReadInt8(&info);
                                    decodingRLE = count & 0x80;
                                    count &= 0x7F;

                                    ReadBytes(&info, channels, sizeof(color) - 1);
                                }

                                *pixelsPtr = (channels[0] << 0) | (channels[1] << 8) | (channels[2] << 16) | (0xFF << 24);
                                pixelsPtr++;

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

                            uint8 count        = 0;
                            bool32 decodingRLE = false;
                            for (int32 p = 0; p < height * width; ++p) {
                                if (count) {
                                    if (!decodingRLE)
                                        ReadBytes(&info, channels, sizeof(uint32));

                                    --count;
                                }
                                else {
                                    uint8 count = ReadInt8(&info);
                                    decodingRLE = count & 0x80;
                                    count &= 0x7F;

                                    ReadBytes(&info, channels, sizeof(color));
                                }

                                *pixelsPtr = (channels[0] << 0) | (channels[1] << 8) | (channels[2] << 16) | (channels[3] << 24);
                                pixelsPtr++;

                                if (reverse && ++x == width) {
                                    x = 0;
                                    pixelsPtr -= width << 1;
                                }
                            }
                            break;
                        }
                    }
                    break;
            }

            Close();
            return true;
        }
    }

    return false;
}
#endif

uint16 RSDK::LoadSpriteSheet(const char *filename, int32 scope)
{
    char fullFilePath[0x100];
    sprintf_s(fullFilePath, (int32)sizeof(fullFilePath), "Data/Sprites/%s", filename);

    RETRO_HASH_MD5(hash);
    GEN_HASH_MD5(filename, hash);

    for (int32 i = 0; i < SURFACE_COUNT; ++i) {
        if (HASH_MATCH_MD5(gfxSurface[i].hash, hash)) {
            return i;
        }
    }

    uint16 id = -1;
    for (id = 0; id < SURFACE_COUNT; ++id) {
        if (gfxSurface[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= SURFACE_COUNT)
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
        image.Close();

        return id;
    }
    else {
        image.palette = NULL;
        image.pixels  = NULL;
        image.decoder = NULL;
        image.Close();
        return -1;
    }
}

bool32 RSDK::LoadImage(const char *filename, double displayLength, double speed, bool32 (*skipCallback)())
{
    char fullFilePath[0x100];
    sprintf_s(fullFilePath, (int32)sizeof(fullFilePath), "Data/Images/%s", filename);

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

        engine.displayTime        = displayLength;
        engine.storedShaderID     = videoSettings.shaderID;
        engine.storedState        = sceneInfo.state;
        videoSettings.dimMax      = 0.0;
        videoSettings.shaderID    = SHADER_RGB_IMAGE;
        videoSettings.screenCount = 0; // "Image Display Mode"
        engine.skipCallback       = skipCallback;
        sceneInfo.state           = ENGINESTATE_SHOWIMAGE;
        engine.imageDelta         = speed / 60.0;

        image.palette = NULL;
        image.pixels  = NULL;
        image.Close();
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

        engine.displayTime        = displayLength;
        engine.storedShaderID     = videoSettings.shaderID;
        engine.storedState        = sceneInfo.state;
        videoSettings.dimMax      = 0.0;
        videoSettings.shaderID    = SHADER_RGB_IMAGE;
        videoSettings.screenCount = 0; // "Image Display Mode"
        engine.skipCallback       = skipCallback;
        sceneInfo.state           = ENGINESTATE_SHOWIMAGE;
        engine.imageDelta         = speed / 60.0;

        image.palette = NULL;
        image.pixels  = NULL;
        image.Close();
        return true;
    }
#endif
    else {
        image.palette = NULL;
        image.pixels  = NULL;
        image.Close();
    }
    return false;
}
