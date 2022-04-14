#include "RSDK/Core/RetroEngine.hpp"

RSDK::SpriteAnimation RSDK::spriteAnimationList[SPRFILE_COUNT];

uint16 RSDK::LoadSpriteAnimation(const char *filename, Scopes scope)
{
    char buffer[0x100];
    sprintf(buffer, "Data/Sprites/%s", filename);

    RETRO_HASH(hash);
    GEN_HASH(filename, hash);

    for (int32 i = 0; i < SPRFILE_COUNT; ++i) {
        if (HASH_MATCH(spriteAnimationList[i].hash, hash)) {
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

    char nameBuffer[0x8][0x20];
    uint8 sheetIDs[0x18];
    sheetIDs[0] = 0;

    FileInfo info;
    InitFileInfo(&info);
    if (LoadFile(&info, buffer, FMODE_RB)) {
        uint32 sig = ReadInt32(&info, false);

        if (sig != 0x525053) {
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
            ReadString(&info, buffer);
            sheetIDs[s] = LoadSpriteSheet(buffer, scope);
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
            GEN_HASH(textBuffer, animation->hash);

            animation->frameCount      = ReadInt16(&info);
            animation->frameListOffset = frameID;
            animation->animationSpeed  = ReadInt16(&info);
            animation->loopIndex       = ReadInt8(&info);
            animation->rotationFlag    = ReadInt8(&info);

            for (int32 f = 0; f < animation->frameCount; ++f) {
                SpriteFrame *frame = &spr->frames[frameID++];
                frame->sheetID     = sheetIDs[ReadInt8(&info)];
                frame->duration    = ReadInt16(&info);
                frame->id          = ReadInt16(&info);
                frame->sprX        = ReadInt16(&info);
                frame->sprY        = ReadInt16(&info);
                frame->width       = ReadInt16(&info);
                frame->height      = ReadInt16(&info);
                frame->pivotX      = ReadInt16(&info);
                frame->pivotY      = ReadInt16(&info);

                frame->hitboxCnt = hitboxCount;
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

uint16 RSDK::CreateSpriteAnimation(const char *filename, uint32 frameCount, uint32 animCount, Scopes scope)
{
    char buffer[0x100];
    sprintf(buffer, "Data/Sprites/%s", filename);

    RETRO_HASH(hash);
    GEN_HASH(filename, hash);

    for (int32 i = 0; i < SPRFILE_COUNT; ++i) {
        if (HASH_MATCH(spriteAnimationList[i].hash, hash)) {
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

    AllocateStorage(sizeof(SpriteFrame) * (frameCount > 0x400 ? 0x400 : frameCount), (void **)&spr->frames, DATASET_STG, true);
    AllocateStorage(sizeof(SpriteAnimationEntry) * (animCount > 0x40 ? 0x40 : animCount), (void **)&spr->animations, DATASET_STG, true);

    return id;
}

void RSDK::ProcessAnimation(Animator *animator)
{
    if (animator) {
        if (animator->frames) {
            animator->timer += animator->speed;
            if (animator->frames == (SpriteFrame *)1) {
                while (animator->timer > animator->frameDuration) {
                    ++animator->frameID;
                    animator->timer -= animator->frameDuration;
                    if (animator->frameID >= animator->frameCount)
                        animator->frameID = animator->loopIndex;
                }
            }
            else {
                while (animator->timer > animator->frameDuration) {
                    ++animator->frameID;
                    animator->timer -= animator->frameDuration;
                    if (animator->frameID >= animator->frameCount)
                        animator->frameID = animator->loopIndex;
                    animator->frameDuration = animator->frames[animator->frameID].duration;
                }
            }
        }
    }
}

int32 RSDK::GetStringWidth(uint16 aniFrames, uint16 animID, TextInfo *info, int32 startIndex, int32 length, int32 spacing)
{
    if (aniFrames >= SPRFILE_COUNT)
        return 0;
    if (!info)
        return 0;
    if (!info->text)
        return 0;

    SpriteAnimation *spr = &spriteAnimationList[aniFrames];
    if (animID < spr->animCount) {
        SpriteAnimationEntry *anim = &spr->animations[animID];

        if (startIndex >= 0) {
            if (startIndex >= info->length)
                startIndex = info->length - 1;
        }
        else {
            startIndex = 0;
        }

        if (length > 0) {
            if (length > info->length)
                length = info->length;
        }
        else {
            length = info->length;
        }

        int32 w = 0;
        for (int32 c = startIndex; c < length; ++c) {
            int32 charFrame = info->text[c];
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

void RSDK::SetSpriteString(uint16 aniFrames, uint16 animID, TextInfo *info)
{
    if (aniFrames >= SPRFILE_COUNT)
        return;
    if (!info)
        return;

    SpriteAnimation *spr = &spriteAnimationList[aniFrames];
    if (animID < spr->animCount) {
        SpriteAnimationEntry *anim = &spr->animations[animID];

        for (int32 c = 0; c < info->length; ++c) {
            int32 charVal = info->text[c];
            info->text[c] = -1;
            for (int32 f = 0; f < anim->frameCount; ++f) {
                if (spr->frames[f + anim->frameListOffset].id == charVal) {
                    info->text[c] = f;
                    break;
                }
            }
        }
    }
}
