#ifndef TEXT_H
#define TEXT_H

struct TextInfo {
    uint16 *text; // text
    int16 length; // string length
    int16 size;   // total alloc length
};

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
            while ((uint32)curChar - 'a' < 26) {
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

extern char textBuffer[0x400];
void GenerateHash(uint32 *buffer, int32 len);
void GenerateCRC(uint32 *id, char *fileName);

#define RETRO_HASH(name)     uint32 name[4]
#define HASH_SIZE            (4 * sizeof(uint32))
#define HASH_MATCH(a, b)     (memcmp(a, b, HASH_SIZE) == 0)
#define GEN_HASH(text, hash)                                                                                                                         \
    strcpy(textBuffer, text);                                                                                                                        \
    GenerateHash(hash, (int32)strlen(textBuffer))
#define HASH_COPY(dst, src) memcpy(dst, src, HASH_SIZE)

inline void SetText(TextInfo *textInfo, char *text, uint size)
{
    if (text) {
        textInfo->text = NULL;
        if (*text) {
            int cnt = 0;
            do
                textInfo->length = ++cnt;
            while (text[cnt]);
        }
        if (size && size >= textInfo->length)
            textInfo->size = size;
        else
            textInfo->size = textInfo->length;
        if (!textInfo->size)
            textInfo->size = 1;
        RSDK::AllocateStorage(sizeof(ushort) * textInfo->size, (void **)&textInfo->text, RSDK::DATASET_STR, false);

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
    if (dst == src)
        return;

    int textLen = src->length;
    dst->text   = NULL;
    if (dst->size >= textLen) {
        if (!dst->text) {
            RSDK::AllocateStorage(sizeof(ushort) * dst->size, (void **)&dst->text, RSDK::DATASET_STR, false);
        }
    }
    else {
        dst->size = textLen;
        RSDK::AllocateStorage(sizeof(ushort) * dst->size, (void **)&dst->text, RSDK::DATASET_STR, false);
    }
    
    dst->length = src->length;
    for (int c = 0; c < dst->length; ++c) {
        dst->text[c] = src->text[c];
    }
}

inline void GetCString(char *dest, TextInfo *info)
{
    if (!info->text)
        return;

    char* text = (char*)"";
    if (dest)
        text = dest;
    int textLen = 0x400;
    if (dest)
        textLen = info->length;
    
    int c = 0;
    for (; c < textLen; ++c) {
        text[c] = info->text[c];
    }
    text[c] = 0;
}

void AppendText(TextInfo *info, char *text);
void PrependText(TextInfo *info, char *text);
void AppendString(TextInfo *textA, TextInfo *textB);
bool32 StringCompare(TextInfo *textA, TextInfo *textB, bool32 flag);
bool32 SplitStringList(TextInfo *list, TextInfo *strings, int32 start, int32 count);

void LoadStrings(TextInfo *buffer, const char *filePath);

#endif
