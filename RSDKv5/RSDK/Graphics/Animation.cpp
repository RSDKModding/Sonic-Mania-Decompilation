#include "RSDK/Core/RetroEngine.hpp"

using namespace RSDK;

SpriteAnimation RSDK::spriteAnimationList[SPRFILE_COUNT];

uint16 RSDK::LoadSpriteAnimation(const char *filePath, int32 scope)
{
    char fullFilePath[0x100];
    sprintf_s(fullFilePath, (int32)sizeof(fullFilePath), "Data/Sprites/%s", filePath);

    RETRO_HASH_MD5(hash);
    GEN_HASH_MD5(filePath, hash);

    for (int32 i = 0; i < SPRFILE_COUNT; ++i) {
        if (HASH_MATCH_MD5(spriteAnimationList[i].hash, hash))
            return i;
    }

    uint16 id = -1;
    for (id = 0; id < SPRFILE_COUNT; ++id) {
        if (spriteAnimationList[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= SPRFILE_COUNT)
        return -1;

    char nameBuffer[0x8][0x20];
    uint8 sheetIDs[0x18];
    sheetIDs[0] = 0;

    FileInfo info;
    InitFileInfo(&info);
    if (LoadFile(&info, fullFilePath, FMODE_RB)) {
        uint32 sig = ReadInt32(&info, false);

        if (sig != RSDK_SIGNATURE_SPR) {
            CloseFile(&info);
            return -1;
        }

        SpriteAnimation *spr = &spriteAnimationList[id];
        spr->scope           = scope;
        memcpy(spr->hash, hash, 4 * sizeof(uint32));

        uint32 frameCount = ReadInt32(&info, false);
        AllocateStorage(frameCount * sizeof(SpriteFrame), (void **)&spr->frames, DATASET_STG, false);

        uint8 sheetCount = ReadInt8(&info);
        for (int32 s = 0; s < sheetCount; ++s) {
            ReadString(&info, fullFilePath);
            sheetIDs[s] = LoadSpriteSheet(fullFilePath, scope);
        }

        uint8 hitboxCount = ReadInt8(&info);
        for (int32 h = 0; h < hitboxCount; ++h) {
            ReadString(&info, nameBuffer[h]);
        }

        spr->animCount = ReadInt16(&info);
        AllocateStorage(spr->animCount * sizeof(SpriteAnimationEntry), (void **)&spr->animations, DATASET_STG, false);

        int32 frameID = 0;
        for (int32 a = 0; a < spr->animCount; ++a) {
            SpriteAnimationEntry *animation = &spr->animations[a];
            ReadString(&info, textBuffer);
            GEN_HASH_MD5(textBuffer, animation->hash);

            animation->frameCount      = ReadInt16(&info);
            animation->frameListOffset = frameID;
            animation->animationSpeed  = ReadInt16(&info);
            animation->loopIndex       = ReadInt8(&info);
            animation->rotationStyle   = ReadInt8(&info);

            for (int32 f = 0; f < animation->frameCount; ++f) {
                SpriteFrame *frame = &spr->frames[frameID++];

                frame->sheetID     = sheetIDs[ReadInt8(&info)];
                frame->duration    = ReadInt16(&info);
                frame->unicodeChar = ReadInt16(&info);
                frame->sprX        = ReadInt16(&info);
                frame->sprY        = ReadInt16(&info);
                frame->width       = ReadInt16(&info);
                frame->height      = ReadInt16(&info);
                frame->pivotX      = ReadInt16(&info);
                frame->pivotY      = ReadInt16(&info);

                frame->hitboxCount = hitboxCount;
                for (int32 h = 0; h < hitboxCount; ++h) {
                    frame->hitboxes[h].left   = ReadInt16(&info);
                    frame->hitboxes[h].top    = ReadInt16(&info);
                    frame->hitboxes[h].right  = ReadInt16(&info);
                    frame->hitboxes[h].bottom = ReadInt16(&info);
                }
            }
        }

        CloseFile(&info);

        return id;
    }

    return -1;
}

uint16 RSDK::CreateSpriteAnimation(const char *filename, uint32 frameCount, uint32 animCount, int32 scope)
{
    char fullFilePath[0x100];
    sprintf_s(fullFilePath, (int32)sizeof(fullFilePath), "Data/Sprites/%s", filename);

    RETRO_HASH_MD5(hash);
    GEN_HASH_MD5(filename, hash);

    for (int32 i = 0; i < SPRFILE_COUNT; ++i) {
        if (HASH_MATCH_MD5(spriteAnimationList[i].hash, hash)) {
            return i;
        }
    }

    uint16 id = -1;
    for (id = 0; id < SPRFILE_COUNT; ++id) {
        if (spriteAnimationList[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= SPRFILE_COUNT)
        return -1;

    SpriteAnimation *spr = &spriteAnimationList[id];
    spr->scope           = scope;
    memcpy(spr->hash, hash, 4 * sizeof(uint32));

    AllocateStorage(sizeof(SpriteFrame) * minVal(frameCount, SPRITEFRAME_COUNT), (void **)&spr->frames, DATASET_STG, true);
    AllocateStorage(sizeof(SpriteAnimationEntry) * minVal(animCount, SPRITEANIM_COUNT), (void **)&spr->animations, DATASET_STG, true);

    return id;
}

void RSDK::ProcessAnimation(Animator *animator)
{
    if (!animator || !animator->frames)
        return;

    animator->timer += animator->speed;

    if (animator->frames == (SpriteFrame *)1) { // model anim
        while (animator->timer > animator->frameDuration) {
            ++animator->frameID;

            animator->timer -= animator->frameDuration;
            if (animator->frameID >= animator->frameCount)
                animator->frameID = animator->loopIndex;
        }
    }
    else { // sprite anim
        while (animator->timer > animator->frameDuration) {
            ++animator->frameID;

            animator->timer -= animator->frameDuration;
            if (animator->frameID >= animator->frameCount)
                animator->frameID = animator->loopIndex;

            animator->frameDuration = animator->frames[animator->frameID].duration;
        }
    }
}

int32 RSDK::GetStringWidth(uint16 aniFrames, uint16 animID, String *string, int32 startIndex, int32 length, int32 spacing)
{
    if (aniFrames >= SPRFILE_COUNT || !string || !string->chars)
        return 0;

    SpriteAnimation *spr = &spriteAnimationList[aniFrames];
    if (animID < spr->animCount) {
        SpriteAnimationEntry *anim = &spr->animations[animID];

        startIndex = clampVal(startIndex, 0, string->length - 1);

        if (length <= 0 || length > string->length)
            length = string->length;

        int32 w = 0;
        for (int32 c = startIndex; c < length; ++c) {
            int32 charFrame = string->chars[c];
            if (charFrame < anim->frameCount) {
                w += spr->frames[charFrame + anim->frameListOffset].width;
                if (c + 1 >= length)
                    return w;

                w += spacing;
            }
        }

        return w;
    }

    return 0;
}

void RSDK::SetSpriteString(uint16 aniFrames, uint16 animID, String *string)
{
    if (aniFrames >= SPRFILE_COUNT || !string)
        return;

    SpriteAnimation *spr = &spriteAnimationList[aniFrames];
    if (animID < spr->animCount) {
        SpriteAnimationEntry *anim = &spr->animations[animID];

        for (int32 c = 0; c < string->length; ++c) {
            int32 unicodeChar = string->chars[c];
            string->chars[c]  = -1;
            for (int32 f = 0; f < anim->frameCount; ++f) {
                if (spr->frames[f + anim->frameListOffset].unicodeChar == unicodeChar) {
                    string->chars[c] = f;
                    break;
                }
            }
        }
    }
}
