#ifndef TEXT_H
#define TEXT_H

struct TextInfo {
    ushort* text;
    short textLength;
    short length;
};

inline void StrCopy(char *dest, const char *src)
{
#ifdef USE_STDLIB
    strcpy(dest, src);
#else
    int i = 0;

    for (; src[i]; ++i) dest[i] = src[i];

    dest[i] = 0;
#endif
}

inline void StrAdd(char *dest, const char *src)
{
    int destStrPos = 0;
    int srcStrPos  = 0;
    while (dest[destStrPos]) ++destStrPos;
    while (true) {
        if (!src[srcStrPos]) {
            break;
        }
        dest[destStrPos++] = src[srcStrPos++];
    }
    dest[destStrPos] = 0;
}

inline bool32 StrComp(const char *stringA, const char *stringB)
{
    bool32 match    = true;
    bool32 finished = false;
    while (!finished) {
        if (*stringA == *stringB || *stringA == *stringB + ' ' || *stringA == *stringB - ' ') {
            if (*stringA) {
                ++stringA;
                ++stringB;
            }
            else {
                finished = true;
            }
        }
        else {
            match    = false;
            finished = true;
        }
    }
    return match;
}

inline int StrLength(const char *string)
{
#ifdef USE_STDLIB
    return strlen(string);
#else
    int len = 0;
    for (len = 0; string[len]; len++)
        ;
    return len;

#endif
}

inline void StringLowerCase(char *dest, const char *src)
{
    int destPos = 0;
    int curChar = *src;
    if (*src) {
        int srcPos = 0;
        do {
            while (curChar - 'A' <= 0x19u) {
                destPos       = srcPos;
                dest[destPos] = curChar + ' ';
                curChar       = src[++srcPos];
                if (!curChar) {
                    dest[++destPos] = 0;
                    return;
                }
            }
            destPos       = srcPos;
            dest[destPos] = curChar;
            curChar       = src[++srcPos];
        } while (curChar);
    }
    dest[++destPos] = 0;
}

inline void StringUpperCase(char *dest, const char *src)
{
    int destPos = 0;
    int curChar = *src;
    if (*src) {
        int srcPos = 0;
        do {
            while (curChar - 'a' <= 0x19u) {
                destPos       = srcPos;
                dest[destPos] = curChar - ' ';
                curChar       = src[++srcPos];
                if (!curChar) {
                    dest[++destPos] = 0;
                    return;
                }
            }
            destPos       = srcPos;
            dest[destPos] = curChar;
            curChar       = src[++srcPos];
        } while (curChar);
    }
    dest[++destPos] = 0;
}

extern char hashBuffer[0x400];
void GenerateHash(uint *buffer, int len);
void GenerateCRC(uint *id, char *fileName);

#define HASH_SIZE            (4 * sizeof(uint))
#define HASH_MATCH(a, b)     (memcmp(a, b, HASH_SIZE) == 0)
#define GEN_HASH(text, hash) StrCopy(hashBuffer, text); GenerateHash(hash, StrLength(hashBuffer))
#define HASH_COPY(dst, src) memcpy(dst, src, 0x10)

extern char textBuffer[0x400];
inline void SetText(TextInfo *textInfo, char *text, uint size)
{
    if (text) {
        if (*text) {
            int cnt = 0;
            do
                textInfo->textLength = ++cnt;
            while (text[cnt]);
        }
        if (size && size >= textInfo->textLength)
            textInfo->length = size;
        else
            textInfo->length = textInfo->textLength;
        if (!textInfo->length)
            textInfo->length = 1;
        AllocateStorage(sizeof(ushort) * textInfo->length, (void **)textInfo, DATASET_STR, false);

        char *txt = text;
        if (*text) {
            int cnt = 0;
            do {
                textInfo->text[cnt++] = *txt;
                ++txt;
            } while (*txt);
        }
    }
}

inline void CopyString(TextInfo *dst, TextInfo *src)
{
    int textLen = src->textLength;
    if (dst->length >= textLen) {
        if (!dst->text) {
            AllocateStorage(sizeof(ushort) * dst->length, (void **)dst, DATASET_STR, 0);
        }
    }
    else {
        dst->length = textLen;
        AllocateStorage(sizeof(ushort) * dst->length, (void **)dst, DATASET_STR, 0);
    }
    
    dst->textLength = src->textLength;
    for (int c = 0; c < dst->textLength; ++c) {
        dst->text[c] = src->text[c];
    }
}

inline void GetCString(char *dest, TextInfo *info)
{
    if (!info->text)
        return;

    char* text = textBuffer;
    if (dest)
        text = dest;
    int textLen = 0x400;
    if (dest)
        textLen = info->textLength;
    
    int c = 0;
    for (; c < textLen; ++c) {
        text[c] = info->text[c];
    }
    text[c] = 0;
}


void Unknown64(TextInfo *textA, TextInfo *textB);
void AppendString(TextInfo *textA, TextInfo *textB);
bool32 StringCompare(TextInfo *textA, TextInfo *textB, byte a3);
void SplitStringList(TextInfo *list, TextInfo *strings, int start, int count);

void LoadStrings(TextInfo *buffer, const char *filePath);

#endif
