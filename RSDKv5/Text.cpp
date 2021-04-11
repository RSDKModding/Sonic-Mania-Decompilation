#include "RetroEngine.hpp"



// From here: https://rosettacode.org/wiki/MD5#C

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef union uwb {
    unsigned w;
    unsigned char b[4];
} WBunion;

typedef unsigned digest[4];

unsigned f0(unsigned abcd[]) { return (abcd[1] & abcd[2]) | (~abcd[1] & abcd[3]); }

unsigned f1(unsigned abcd[]) { return (abcd[3] & abcd[1]) | (~abcd[3] & abcd[2]); }

unsigned f2(unsigned abcd[]) { return abcd[1] ^ abcd[2] ^ abcd[3]; }

unsigned f3(unsigned abcd[]) { return abcd[2] ^ (abcd[1] | ~abcd[3]); }

typedef unsigned (*DgstFctn)(unsigned a[]);

unsigned *calcKs(unsigned *k)
{
    double s, pwr;
    int i;

    pwr = pow(2, 32);
    for (i = 0; i < 64; i++) {
        s    = fabs(sin(1 + i));
        k[i] = (unsigned)(s * pwr);
    }
    return k;
}

// ROtate v Left by amt bits
unsigned rol(unsigned v, short amt)
{
    unsigned msk1 = (1 << amt) - 1;
    return ((v >> (32 - amt)) & msk1) | ((v << amt) & ~msk1);
}

unsigned *md5(const char *msg, int mlen)
{
    static digest h0 = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476 };
    //    static Digest h0 = { 0x01234567, 0x89ABCDEF, 0xFEDCBA98, 0x76543210 };
    static DgstFctn ff[] = { &f0, &f1, &f2, &f3 };
    static short M[]     = { 1, 5, 3, 7 };
    static short O[]     = { 0, 1, 5, 0 };
    static short rot0[]  = { 7, 12, 17, 22 };
    static short rot1[]  = { 5, 9, 14, 20 };
    static short rot2[]  = { 4, 11, 16, 23 };
    static short rot3[]  = { 6, 10, 15, 21 };
    static short *rots[] = { rot0, rot1, rot2, rot3 };
    static unsigned kspace[64];
    static unsigned *k;

    static digest h;
    digest abcd;
    DgstFctn fctn;
    short m, o, g;
    unsigned f;
    short *rotn;
    union {
        unsigned w[16];
        char b[64];
    } mm;
    int os = 0;
    int grp, grps, q, p;
    unsigned char *msg2;

    if (k == NULL)
        k = calcKs(kspace);

    for (q = 0; q < 4; q++) h[q] = h0[q]; // initialize

    {
        grps = 1 + (mlen + 8) / 64;
        msg2 = (unsigned char *)malloc(64 * grps);
        memcpy(msg2, msg, mlen);
        msg2[mlen] = (unsigned char)0x80;
        q          = mlen + 1;
        while (q < 64 * grps) {
            msg2[q] = 0;
            q++;
        }
        {
            //            unsigned char t;
            WBunion u;
            u.w = 8 * mlen;
            //            t = u.b[0]; u.b[0] = u.b[3]; u.b[3] = t;
            //            t = u.b[1]; u.b[1] = u.b[2]; u.b[2] = t;
            q -= 8;
            memcpy(msg2 + q, &u.w, 4);
        }
    }

    for (grp = 0; grp < grps; grp++) {
        memcpy(mm.b, msg2 + os, 64);
        for (q = 0; q < 4; q++) abcd[q] = h[q];
        for (p = 0; p < 4; p++) {
            fctn = ff[p];
            rotn = rots[p];
            m    = M[p];
            o    = O[p];
            for (q = 0; q < 16; q++) {
                g = (m * q + o) % 16;
                f = abcd[1] + rol(abcd[0] + fctn(abcd) + k[q + 16 * p] + mm.w[g], rotn[q % 4]);

                abcd[0] = abcd[3];
                abcd[3] = abcd[2];
                abcd[2] = abcd[1];
                abcd[1] = f;
            }
        }
        for (p = 0; p < 4; p++) h[p] += abcd[p];
        os += 64;
    }

    if (msg2)
        free(msg2);

    return h;
} 

char hashBuffer[0x400];
// Buffer is expected to be at least 16 bytes long
void GenerateHash(uint *buffer, int len)
{
    byte *buf   = (byte *)buffer;
    unsigned *d = md5(hashBuffer, len);
    WBunion u;

    for (int i = 0; i < 4; ++i) {
        u.w = d[i];
        for (int c = 0; c < 4; ++c) buf[(i << 2) + c] = u.b[c];
    }
}

uint crc32_t[256] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e,
    0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb,
    0xf4d4b551, 0x83d385c7, 0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5, 0x3b6e20c8,
    0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
    0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59, 0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599,
    0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
    0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433, 0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb,
    0x086d3d2d, 0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
    0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65, 0x4db26158, 0x3ab551ce, 0xa3bc0074,
    0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5,
    0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f, 0x5edef90e,
    0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27,
    0x7d079eb1, 0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0,
    0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1,
    0xa6bc5767, 0x3fb506dd, 0x48b2364b, 0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
    0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92,
    0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d, 0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
    0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4,
    0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777, 0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
    0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d,
    0x3e6e77db, 0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9, 0xbdbdf21c, 0xcabac28a,
    0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37,
    0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

void GenerateCRC(uint *id, char *fileName)
{
    *id     = -1;
    if (!fileName)
        return;
    int len       = 0;
    do
        ++len;
    while (fileName[len]);

    for (uint i = 0; i < len; i++) {
        *id = crc32_t[(byte)(*id ^ *fileName)] ^ (*id >> 8);
        fileName++;
    }
    *id = ~*id;
    return;
}

byte stringFlags[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                       1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                       2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6 };

void AppendString(TextInfo *textA, TextInfo *textB)
{
    uint charID   = 0;
    uint totalLen = textB->textLength + textA->textLength;
    if (textA->length < totalLen || !textA->text) {
        AllocateStorage(sizeof(ushort) * totalLen, (void **)&textA, DATASET_STR, 0);
        if (textA->textLength > 0) {
            do {
                textA->text[charID] = textA->text[charID];
                ++charID;
            } while (charID < textA->textLength);
        }
        CopyStorage((int **)textA, (int **)&textA);
        textA->length = textB->textLength + textA->textLength;
    }

    int textLen           = textA->textLength;
    textA->textLength = textLen + textB->textLength;
    if (textLen < textA->textLength) {
        int id = 0;
        do {
            textA->text[textLen++] = textB->text[id];
            ++id;
        } while (textLen < textA->textLength);
    }
}

bool32 StringCompare(TextInfo *textA, TextInfo *textB, byte a3)
{
    int lengthA = textA->textLength;
    if (lengthA != textB->textLength)
        return 0;
    ushort *textPtrA = textA->text;
    int id           = 0;
    ushort *textPtrB = textB->text;
    if (a3) {
        if (lengthA > 0u) {
            int dif = (char *)textPtrB - (char *)textPtrA;
            while (*textPtrA == textPtrA[dif]) {
                ++id;
                ++textPtrA;
                if (id >= lengthA)
                    return true;
            }
            return false;
        }
        return true;
    }

    if (lengthA <= 0)
        return true;
    int dif = (char *)textPtrB - (char *)textPtrA;
    while (true) {
        int v9 = textPtrA[dif];
        if (*textPtrA != v9) {
            ushort cur = *textPtrA;
            if (cur != v9 + ' ' && cur != v9 - ' ')
                break;
        }
        ++id;
        ++textPtrA;
        if (id >= lengthA)
            return true;
    }
    return false;
}

void SplitStringList(TextInfo *list, TextInfo *strings, int start, int count)
{
    if (!strings->length || !strings->text)
        return;

    int lastStrPos = 0;
    int strID      = 0;

    TextInfo *info = list;
    for (int c = 0; c < strings->textLength && count > 0; ++c) {
        if (strings->text[c] == '\n') {
            if (strID < start) {
                lastStrPos = c;
            }
            else {
                ushort len = c - lastStrPos;
                if (info->length < len) {
                    info->length = len;
                    AllocateStorage(sizeof(ushort) * len, (void **)&info->text, DATASET_STR, true);
                }
                info->textLength = len;

                for (int i = 0; i < info->textLength; ++i) {
                    info->text[i] = strings->text[lastStrPos++];
                }

                ++info;
                --count;
            }
            ++strID;
            ++lastStrPos;
        }
    }
}

void InitStringsBuffer(TextInfo *info, int size)
{
    ushort *text = NULL; 

    AllocateStorage(sizeof(ushort) * size, (void **)&text, DATASET_STR, 0);
    
    for (int i = 0; i < size && i < info->textLength; ++i) {
        text[i] = info->text[i];
    }
    
    CopyStorage((int **)info, (int **)&text);
    info->length = size;
    if (info->textLength > (ushort)size)
        info->textLength = size;
}

void LoadStrings(TextInfo *buffer, const char *filename)
{
    char nameBuf[0x100];
    sprintf(nameBuf, "Data/Strings/%s", filename);
    
    FileInfo info;
    MEM_ZERO(info);
    if (LoadFile(&info, nameBuf, FMODE_RB)) {
        ushort header = ReadInt16(&info);
        if (header == 0xFEFF) {
            InitStringsBuffer(buffer, (info.fileSize >> 1) - 1);
            ReadBytes(&info, buffer->text, buffer->length * sizeof(ushort));
            buffer->textLength = buffer->length;
        }
        else {
            if (header == 0xEFBB) {
                ReadInt8(&info);
            }
            else {
                Seek_Set(&info, 0);
            }
            InitStringsBuffer(buffer, info.fileSize);

            for (int i = 0; i < info.fileSize; ++i) {
                int curChar = 0;

                byte buf = ReadInt8(&info);
                switch (stringFlags[buf]) {
                    case 1: curChar = buf; break;
                    case 2:
                        curChar = ((buf & 0x1F) << 6);
                        curChar |= (ReadInt8(&info) & 0x3F);
                        break;
                    case 3:
                        curChar = (buf << 12);
                        curChar |= ((ReadInt8(&info) & 0x3F) << 6);
                        curChar |= ReadInt8(&info) & 0x3F;
                        break;
                    case 4:
                        curChar = ReadInt8(&info) << 12;
                        curChar |= ((ReadInt8(&info) & 0x3F) << 6);
                        curChar |= ReadInt8(&info) & 0x3F;
                        break;
                    case 5:
                        i += 4;
                        Seek_Cur(&info, 4);
                        break;
                    case 6:
                        i += 5;
                        Seek_Cur(&info, 5);
                        break;
                    default: break;
                }

                buffer->text[buffer->textLength++] = curChar;
            }
        }

        CloseFile(&info);
    }
}