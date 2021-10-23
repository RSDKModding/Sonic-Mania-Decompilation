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
void PNGUnpackGreyscale(ImagePNG *image, byte *data)
{
    byte *pixels = image->dataPtr;
    for (int c = 0; c < image->width * image->height; ++c) {
        int a = *data;
        int b = *data++;
        *pixels = a | ((a | ((b | 0xFFFFFF00) << 8)) << 8);
        pixels += 4;
    }
}

void PNGUnpackGreyscaleA(ImagePNG *image, byte *data)
{
    byte *pixels = image->dataPtr;
    for (int c = 0; c < image->width * image->height; ++c) {
        int val = *data++;
        *pixels = val | ((val | (val << 8)) << 8) | ((val | ((val | (val << 8)) << 8)) << 24);
        ++pixels;
    }
}

void PNGUnpackRGB(ImagePNG *image, byte *data)
{
    byte *pixels = image->dataPtr;
    for (int c = 0; c < image->width * image->height; ++c) {
        *pixels = (0xFFFFFF00 | *data) << 16;
        data++;
        *pixels |= *data << 8;
        data++;
        *pixels |= *data << 0;
        data++;
        pixels += 4;
    }
}

void PNGUnpackRGBA(ImagePNG *image, byte *data)
{
    byte *pixels = image->dataPtr;
    for (int c = 0; c < image->width * image->height; ++c) {
        *pixels |= *data << 16;
        data++;
        *pixels |= *data << 8;
        data++;
        *pixels |= *data << 0;
        data++;
        *pixels = *data << 24;
        data++;
        pixels += 4;
    }
}

//TODO: cleanup
void PNGDecodeData(ImagePNG *image, byte *dataPtr) {
    signed int v2; 
    int v4;        
    byte *result;  
    int v6;        
    byte v7;       
    signed int v8; 
    byte v9;       
    int v10;       
    byte *v11;     
    int v12;       
    int v13;       
    char v14;      
    byte v15;      
    signed int v16;
    byte v17;      
    byte *v18;       
    int v19;       
    byte v20;      
    char v22;      
    signed int v23;
    byte v24;      
    byte *v25;       
    int v26;       
    byte v27;      
    byte* v28;       
    byte v29;      
    byte *v30;     
    signed int v31;
    byte v32;      
    byte v33;      
    int v34;       
    byte *v35;       
    int v36;       
    int v37;       
    byte *v38;     
    int v39;       
    byte v40;      
    byte v41;      
    int v42;       
    byte *v43;     
    int v44;       
    int v45;       
    int v46;       
    int v47;       
    int v48;       
    int v49;       
    int v50;       
    int v51;       
    char v52;      
    byte v53;      
    bool32 v54;      
    int v55;       
    byte v56;      
    int v57;       
    int v58;       
    int v59;       
    int v61;       
    int v62;       
    int v63;       
    byte* v64;       
    int v65;       
    int v66;       
    int v67;       
    int v68;       
    int v70;        

    v2 = ((uint)image->bitDepth + 7) >> 3;
    v2 = ((uint)image->bitDepth + 7) >> 3;
    switch (image->clrType) {
        case 2: v2 *= 3; break;
        case 4: v2 *= 2; break;
        case 6: v2 *= 4; break;
    }
    v4     = v2 * image->width;
    v70    = v4;
    v59    = v4 + v2;
    result = (dataPtr + 1);
    if (*dataPtr == 1 || *dataPtr == 3) {
        if (v2) {
            v16 = v2;
            do {
                v17        = *result++;
                *dataPtr++ = v17;
                --v16;
            } while (v16);
        }
        if (v2 < v4) {
            v18 = &dataPtr[-v2];
            v19 = v4 - v2;
            do {
                v20        = *(byte *)(++v18 - 1) + *result++;
                *dataPtr++ = v20;
                --v19;
            } while (v19);
        LABEL_29:
            v4 = v70;
            goto LABEL_30;
        }
    }
    else {
        if (*dataPtr != 4) {
            if (v4 > 0) {
                v6 = v2 * image->width;
                do {
                    v7         = *result++;
                    *dataPtr++ = v7;
                    --v6;
                } while (v6);
            }
            goto LABEL_30;
        }
        if (v2) {
            v8 = v2;
            do {
                v9         = *result++;
                *dataPtr++ = v9;
                --v8;
            } while (v8);
        }
        if (v2 < v4) {
            v10 = v4;
            v11 = &dataPtr[-v2];
            v12 = v10 - v2;
            do {
                v13 = *v11++;
                v14 = 0;
                if (v13 >= 0)
                    v14 = v13;
                v15        = v14 + *result++;
                *dataPtr++ = v15;
                --v12;
            } while (v12);
            goto LABEL_29;
        }
    }
LABEL_30:
    for (int i = 1; i < image->height; ++i) {
        v22 = *result++;
        switch (v22) {
            case 1:
                if (v2) {
                    v23 = v2;
                    do {
                        v24        = *result++;
                        *dataPtr++ = v24;
                        --v23;
                    } while (v23);
                }
                if (v2 < v4) {
                    v25 = &dataPtr[-v2];
                    v26 = v4 - v2;
                    do {
                        v27        = *(byte *)(++v25 - 1) + *result++;
                        *dataPtr++ = v27;
                        --v26;
                    } while (v26);
                    v4 = v70;
                }
                break;
            case 2:
                if (v4 > 0) {
                    v28 = &dataPtr[-v4];
                    do {
                        v29        = *(byte *)(++v28 - 1) + *result++;
                        *dataPtr++ = v29;
                        --v4;
                    } while (v4);
                    v4 = v70;
                }
                break;
            case 3:
                if (v2) {
                    v30 = &dataPtr[-v4];
                    v31 = v2;
                    do {
                        v32        = *v30++;
                        v33        = *result++ + (v32 >> 1);
                        *dataPtr++ = v33;
                        --v31;
                    } while (v31);
                    v4 = v70;
                }
                if (v2 < v4) {
                    v34 = v2 - v70;
                    v35 = &dataPtr[-v2];
                    v36 = v70 - v2;
                    v61 = v34;
                    do {
                        v37         = *(byte *)(v34 + v35++);
                        v34         = v61;
                        v37 = *result++ + (((uint)*(byte *)(v35 - 1) + v37) >> 1);
                        *dataPtr++  = v37;
                        --v36;
                    } while (v36);
                    v4 = v70;
                }
                break;
            case 4:
                if (v2) {
                    v38 = &dataPtr[-v70];
                    do {
                        v39 = *v38;
                        v40 = 0;
                        if (v39 > 0 && v39 >= 0)
                            v40 = *v38;
                        ++v38;
                        v41        = v40 + *result++;
                        *dataPtr++ = v41;
                        --v2;
                    } while (v2);
                    v4 = v70;
                }
                if (v2 < v4) {
                    v42 = v4 - v59;
                    v43 = &dataPtr[-v4];
                    v64 = &dataPtr[-v4];
                    v44 = v4 - v2;
                    v57 = v4 - v59;
                    v62 = v44;
                    v63 = v44;
                    do {
                        v45 = v43[v42];
                        v46 = *v43;
                        v47 = v43[v44];
                        v65 = v46;
                        v66 = v45;
                        v67 = v47 - v45;
                        v48 = v47 - v45 + v46;
                        v49 = v47 - v48;
                        if (v48 > v47)
                            v49 = v48 - v47;
                        v58 = v49;
                        v50 = v67;
                        if (v48 <= v65)
                            v50 = v65 - v48;
                        v68 = v50;
                        v51 = v66 - v48;
                        if (v48 > v66)
                            v51 = v48 - v66;
                        if (v58 > v68 || v58 > v51) {
                            v47 = v66;
                            if (v68 <= v51)
                                v47 = v65;
                        }
                        v52        = *result++;
                        v42        = v57;
                        v53        = v47 + v52;
                        v44        = v62;
                        *dataPtr++ = v53;
                        v43        = (byte *)(v64 + 1);
                        v54        = v63-- == 1;
                        ++v64;
                    } while (!v54);
                    v4 = v70;
                }
                break;
            default:
                if (v4 > 0) {
                    v55 = v4;
                    do {
                        v56        = *result++;
                        *dataPtr++ = v56;
                        --v55;
                    } while (v55);
                }
                break;
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
                        ReadBytes(&image->info, &image->width, 4);
                        ReadBytes(&image->info, &image->height, 4);
                        image->bitDepth    = ReadInt8(&image->info);
                        image->clrType     = ReadInt8(&image->info);
                        image->compression = ReadInt8(&image->info);
                        image->filter      = ReadInt8(&image->info);
                        image->interlaced  = ReadInt8(&image->info);
                        if (image->interlaced || image->bitDepth != 8) {
                            CloseFile(&image->info);
                            return false;
                        }
                        image->depth    = 32;
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
                    else if (/*image->chunkHeader == 'TADI'*/ false) {
                        if (!image->dataPtr) {
                            AllocateStorage(sizeof(uint) * image->height * (image->width + 1), (void **)&image->dataPtr, DATASET_TMP, 0);
                            if (!image->dataPtr) {
                                CloseFile(&image->info);
                                return false;
                            }
                        }
                        AllocateStorage(image->chunkSize, (void **)&image->dataBuffer, DATASET_TMP, 0);
                        ReadBytes(&image->info, image->dataBuffer, image->chunkSize);

                        byte *dataPtr = NULL;
                        switch (image->clrType) {
                            case 0:
                            case 3: dataPtr = &image->dataPtr[3 * image->width * image->height]; break;
                            case 2: dataPtr = &image->dataPtr[image->width * image->height]; break;
                            case 4: dataPtr = &image->dataPtr[2 * image->width * image->height]; break;
                            default: dataPtr = image->dataPtr; break;
                        }

                        image->dataSize = 4 * image->height * (image->width + 1);
                        ReadZLib(&image->info, (byte **)&image->dataBuffer, image->chunkSize, image->dataSize);
                        PNGDecodeData(image, dataPtr);
                        switch (image->clrType) {
                            case 0: PNGUnpackGreyscale(image, dataPtr); break;
                            case 2: PNGUnpackRGB(image, dataPtr); break;
                            case 3:
                                for (int c = 0; c < image->width * image->height; ++c) {
                                    image->dataPtr[c] = image->palette[*dataPtr] | 0xFF000000;
                                    ++dataPtr;
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
bool32 LoadTGA(const char *filename)
{
    FileInfo info;
    MEM_ZERO(info);

    if (LoadFile(&info, filename, FMODE_RB)) {
        byte idlength         = ReadInt8(&info);
        byte colourmaptype    = ReadInt8(&info);
        byte datatypecode     = ReadInt8(&info);
        short colourmaporigin = ReadInt16(&info);
        short colourmaplength = ReadInt16(&info);
        byte colourmapdepth   = ReadInt8(&info);
        short x_origin        = ReadInt16(&info);
        short y_origin        = ReadInt16(&info);
        short width           = ReadInt16(&info);
        short height          = ReadInt16(&info);
        byte bitsperpixel     = ReadInt8(&info);
        byte imagedescriptor  = ReadInt8(&info);
        bool32 flag           = (~imagedescriptor >> 4) & 1;
        if (width == 1024 && height == 512 && bitsperpixel >= 0x10) {
            if (idlength)
                Seek_Cur(&info, idlength);

            uint *imageData = NULL;
            AllocateStorage(0x200000u, (void **)&imageData, DATASET_TMP, false);
            if (flag)
                imageData += 0x7FC00;

            int y = 0;
            if (datatypecode == 2) {
                switch (bitsperpixel) {
                    case 0x10:
                        for (int i = 0; i < 0x80000; ++i) {
                            byte val[2];
                            ReadBytes(&info, val, sizeof(ushort));
                            if (val[0] + (val[1] << 8) < 0)
                                *imageData = 8 * (val[0] & 0x1F | 8 * ((val[0] + (val[1] << 8)) & 0x3E0 | 0x3FC00));
                            else
                                *imageData = 0;
                            ++imageData;
                            if (flag && ++y == 1024) {
                                y = 0;
                                imageData -= 2048;
                            }
                        }
                        break;
                    case 0x18:
                        for (int i = 0; i < 0x80000; ++i) {
                            byte val[3];
                            ReadBytes(&info, val, 3 * sizeof(byte));
                            *imageData = val[0] | ((val[1] | 0xFF00) << 8);
                            ++imageData;
                            if (flag && ++y == 1024) {
                                y = 0;
                                imageData -= 2048;
                            }
                        }
                        break;
                    case 0x20:
                        for (int i = 0; i < 0x80000; ++i) {
                            byte val[4];
                            ReadBytes(&info, val, 4 * sizeof(byte));
                            *imageData = (val[0] << 0) | (val[1] << 8) | (val[2] << 16) | (val[3] << 24);
                            ++imageData;
                            if (flag && ++y == 1024) {
                                y = 0;
                                imageData -= 2048;
                            }
                        }
                        break;
                }
            }
            else if (datatypecode == 10) {
                switch (bitsperpixel) {
                    case 0x10: {
                        ushort value = 0;
                        byte count = 0, flag = 0;
                        for (int i = 0; i < 0x80000; ++i) {
                            if (count) {
                                if (!flag) {
                                    byte val[2];
                                    ReadBytes(&info, val, sizeof(ushort));
                                    value = val[0] + (val[1] << 8);
                                }
                                --count;
                            }
                            else {
                                byte flags = 0;
                                ReadBytes(&info, &flags, sizeof(byte));
                                flag  = flags & 0x80;
                                count = flags & 0x7F;
                                flags &= 0x80;
                                byte val[2];
                                ReadBytes(&info, val, sizeof(ushort));
                                ushort value = val[0] + (val[1] << 8);
                            }

                            if (value < 0) {
                                *imageData = 8 * (value & 0x1F | 8 * (value & 0x3E0 | 0x3FC00));
                            }
                            else {
                                *imageData = 0;
                            }

                            ++imageData;
                            if (flag && ++y == 1024) {
                                y = 0;
                                imageData -= 2048;
                            }
                        }
                    } break;
                    case 0x18: {
                        byte val[3];
                        memset(val, 0, sizeof(val));
                        byte count = 0, flag = 0;
                        for (int i = 0; i < 0x80000; ++i) {
                            if (count) {
                                if (!flag) {
                                    ReadBytes(&info, val, 3 * sizeof(byte));
                                }
                                --count;
                            }
                            else {
                                byte flags = 0;
                                ReadBytes(&info, &flags, sizeof(byte));
                                flag  = flags & 0x80;
                                count = flags & 0x7F;
                                flags &= 0x80;
                                byte val[2];
                                ReadBytes(&info, val, sizeof(ushort));
                            }
                            *imageData = val[0] | ((val[1] | 0xFF00) << 8);

                            ++imageData;
                            if (flag && ++y == 1024) {
                                y = 0;
                                imageData -= 2048;
                            }
                        }
                    } break;
                    case 0x20: {
                        byte val[sizeof(uint)];
                        memset(val, 0, sizeof(val));
                        byte count = 0, flag = 0;
                        for (int i = 0; i < 0x80000; ++i) {
                            if (count) {
                                if (!flag) {
                                    ReadBytes(&info, val, sizeof(uint));
                                }
                                --count;
                            }
                            else {
                                byte flags = 0;
                                ReadBytes(&info, &flags, sizeof(byte));
                                flag  = flags & 0x80;
                                count = flags & 0x7F;
                                flags &= 0x80;
                                byte val[2];
                                ReadBytes(&info, val, sizeof(ushort));
                            }
                            *imageData = (val[0] << 0) | (val[1] << 8) | (val[2] << 16) | (val[3] << 24);

                            ++imageData;
                            if (flag && ++y == 1024) {
                                y = 0;
                                imageData -= 2048;
                            }
                        }
                    } break;
                }
            }
            CloseFile(&info);
            //SetImageTexture(512, 1024, imageData);
            //sub_5AD370(&imageData);
            imageData = NULL;
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

#if RETRO_USING_OPENGL
        glGenTextures(1, &gfxSurface[id].id);
        glBindTexture(GL_TEXTURE_2D, gfxSurface[id].id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, surface->width, surface->height, 0, GL_RED, GL_UNSIGNED_BYTE, gfxSurface[id].dataPtr);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif

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

bool32 LoadImage(const char *filename, double displayTime, double delta, bool32 (*skipCallback)(void))
{
    char buffer[0x100];
    sprintf(buffer, "Data/Images/%s", filename);

    ImagePNG image;
    MEM_ZERO(image);
    InitFileInfo(&image.info);

#if RETRO_REV02
    if (LoadPNG(&image, buffer, false)) {
        if (image.width == 1024 && image.height == 512)
           SetImageTexture(512, 1024, image.dataPtr);

        engine.displayTime    = displayTime;
        engine.prevShaderID   = engine.shaderID;
        engine.prevEngineMode = sceneInfo.state;
        engine.dimMax         = 0.0;
        engine.shaderID       = SHADER_RGB_IMAGE;
        engine.screenCount    = 0;
        engine.skipCallback   = skipCallback;
        sceneInfo.state       = ENGINESTATE_SHOWPNG;
        engine.imageDelta     = delta / 60.0;

        image.palette = NULL;
        image.dataPtr = NULL;
        CloseFile(&image.info);
        return true;
    }
    else {
        image.palette = NULL;
        image.dataPtr = NULL;
        CloseFile(&image.info);
    }
#elif !RETRO_REV02
    if (LoadTGA(buffer)) {
        if (image.width == 1024 && image.height == 512)
            SetImageTexture(512, 1024, image.dataPtr);

        engine.displayTime    = displayTime;
        engine.prevShaderID   = engine.shaderID;
        engine.prevEngineMode = sceneInfo.state;
        engine.dimMax         = 0.0;
        engine.shaderID       = SHADER_RGB_IMAGE;
        engine.screenCount    = 0;
        engine.skipCallback   = skipCallback;
        sceneInfo.state       = ENGINESTATE_SHOWPNG;
        engine.imageDelta     = delta / 60.0;
        return true;
    }
#endif
    return false;
}
