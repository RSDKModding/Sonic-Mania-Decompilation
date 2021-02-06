#include "RetroEngine.hpp"

SpriteAnimation animationFileList[SPRFILE_COUNT];
SpriteFrame animFrames[SPRITEFRAME_COUNT];
int animFrameCount;
SpriteAnimationEntry animationList[ANIMATION_COUNT];
int animationCount;

short LoadAnimation(const char *filename, Scopes scope)
{
    char buffer[0x100];
    FileInfo info;
    StrCopy(buffer, "Data/Sprites/");
    StrAdd(buffer, filename);

    uint hash[4];
    StrCopy(hashBuffer, filename);
    GenerateHash(hash, StrLength(filename));

    for (int i = 0; i < SPRFILE_COUNT; ++i) {
        if (memcmp(hash, animationFileList[i].hash, 4 * sizeof(int)) == 0) {
            return i;
        }
    }

    short id = -1;
    for (id = 0; id < SPRFILE_COUNT; ++id) {
        if (animationFileList[id].scope == SCOPE_NONE)
            break;
    }

    if (id >= SPRFILE_COUNT)
        return -1;

    char nameBuffer[0x8][0x20];
    byte sheetIDs[0x18];
    sheetIDs[0] = 0;
    MEM_ZERO(info);
    if (LoadFile(&info, buffer)) {
        uint sig = ReadInt32(&info);

        if (sig != 0x525053) {
            CloseFile(&info);
            return -1;
        }

        SpriteAnimation *animFile = &animationFileList[id];
        animFile->scope           = scope;
        memcpy(animFile->hash, hash, 4 * sizeof(int));
        animFile->aniListOffset   = animationCount;

        uint frameCount = ReadInt32(&info);
        AllocateStorage(frameCount * sizeof(SpriteFrame), NULL, DATASET_STG, false);

        byte sheetCount = ReadInt8(&info);
        for (int s = 0; s < sheetCount; ++s) {
            ReadString(&info, buffer);
            sheetIDs[s] = LoadSpriteSheet(buffer, scope);
        }

        byte hitboxCount = ReadInt8(&info);
        for (int h = 0; h < hitboxCount; ++h) {
            ReadString(&info, nameBuffer[h]);
        }

        animFile->animCount = ReadInt16(&info);
        for (int a = 0; a < animFile->animCount; ++a) {
            SpriteAnimationEntry *animation = &animationList[animationCount++];
            ReadString(&info, hashBuffer);
            GenerateHash(animation->hash, StrLength(hashBuffer));

            animation->frameCount  = ReadInt16(&info);
            animation->frameListOffset = animFrameCount;
            animation->animationSpeed = ReadInt16(&info);
            animation->loopIndex       = ReadInt8(&info);
            animation->rotationFlag   = ReadInt8(&info);

            for (int f = 0; f < animation->frameCount; ++f) {
                SpriteFrame *frame = &animFrames[animFrameCount++];
                frame->sheetID     = sheetIDs[ReadInt8(&info)];
                frame->delay       = ReadInt16(&info);
                frame->id          = ReadInt16(&info);
                frame->sprX        = ReadInt16(&info);
                frame->sprY        = ReadInt16(&info);
                frame->width       = ReadInt16(&info);
                frame->height      = ReadInt16(&info);
                frame->pivotX      = ReadInt16(&info);
                frame->pivotY      = ReadInt16(&info);

                frame->hitboxCnt = hitboxCount;
                for (int h = 0; h < hitboxCount; ++h) {
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

void ProcessAnimation(EntityAnimationData *data)
{
    if (data) {
        if (data->framePtrs) {
            data->animationTimer += data->animationSpeed;
            if (data->framePtrs == (SpriteFrame *)1) {
                int delay = data->frameDelay;
                while (data->animationTimer <= delay) {
                    ++data->frameID;
                    data->animationTimer = data->animationTimer - delay;
                    if (data->frameID >= data->frameCount)
                        data->frameID = data->loopIndex;
                }
            }
            else {
                while (data->animationTimer <= data->frameDelay) {
                    ++data->frameID;
                    data->animationTimer = data->animationTimer - data->frameDelay;
                    if (data->frameID >= data->frameCount)
                        data->frameID = data->loopIndex;
                    data->frameDelay = data->framePtrs[data->frameID].delay;
                }
            }
        }
    }
}