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

char textBuffer[0x400];

void Unknown64(TextInfo *textA, TextInfo *textB)
{
    int textLen = textB->textLength;
    if (textA->length >= textLen) {
        if (!textA->text) {
            AllocateStorage(sizeof(ushort) * textA->length, (void **)textA, DATASET_STR, false);
        }
    }
    else {
        textA->length = textLen;
        AllocateStorage(sizeof(ushort) * textA->length, (void **)textA, DATASET_STR, false);
    }

    textA->textLength = textB->textLength;
    int id                = 0;
    if (textB->textLength > 0u) {
        do {
            textA->text[id] = textB->text[id];
            ++id;
        } while (id < textA->textLength);
    }
}

void Unknown66(TextInfo *textA, TextInfo *textB)
{
    uint charID   = 0;
    uint totalLen = textB->textLength + textA->textLength;
    if (textA->length < totalLen || !textA->text) {
        AllocateStorage(sizeof(ushort) * totalLen, (void **)&textA, DATASET_STR, 0);
        if (textA->textLength > 0u) {
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
            ++id;
            textA->text[textLen++] = textB->text[id - 1];
        } while (textLen < textA->textLength);
    }
}

bool32 Unknown69(TextInfo *textA, TextInfo *textB, byte a3)
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