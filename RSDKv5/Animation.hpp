#ifndef ANIMATION_H
#define ANIMATION_H

#define SPRFILE_COUNT     (0x400)

#define FRAMEHITBOX_COUNT (0x8)

enum RotationFlags { ROTFLAG_NONE, ROTFLAG_FULL, ROTFLAG_2, ROTFLAG_3, ROTFLAG_4, ROTFLAG_STATICFRAMES };

struct SpriteAnimationEntry {
    uint hash[4];
    int frameListOffset;
    ushort frameCount;
    short animationSpeed;
    byte loopIndex;
    byte rotationFlag;
};

struct Hitbox {
    short left;
    short top;
    short right;
    short bottom;
};

struct SpriteFrame {
    ushort sprX;
    ushort sprY;
    ushort width;
    ushort height;
    short pivotX;
    short pivotY;
    ushort delay;
    short id;
    byte sheetID;
    byte hitboxCnt;
    Hitbox hitboxes[FRAMEHITBOX_COUNT];
};

struct SpriteAnimation {
    uint hash[4];
    SpriteFrame *frames;
    SpriteAnimationEntry *animations;
    ushort animCount;
    byte scope;
};

struct Animator {
    SpriteFrame *framePtrs;
    int frameID;
    short animationID;
    short prevAnimationID;
    short animationSpeed;
    short animationTimer;
    short frameDelay;
    short frameCount;
    byte loopIndex;
    byte rotationFlag;
};

extern SpriteAnimation spriteAnimationList[SPRFILE_COUNT];

short LoadAnimation(const char *filename, Scopes scope);
short CreateAnimation(const char *filename, uint frameCount, uint animCount, Scopes scope);

inline ushort GetAnimation(ushort sprIndex, const char *name)
{
    if (sprIndex >= SPRFILE_COUNT)
        return NULL;
    SpriteAnimation *spr = &spriteAnimationList[sprIndex];

    uint hash[4];
    GEN_HASH(name, hash);

    for (int a = 0; a < spr->animCount; ++a) {
        if (HASH_MATCH(hash, spr->animations[a].hash)) {
            return a;
        }
    }
    return -1;
}

inline SpriteFrame *GetFrame(ushort sprIndex, ushort anim, int frame)
{
    if (sprIndex >= SPRFILE_COUNT)
        return NULL;
    SpriteAnimation *spr = &spriteAnimationList[sprIndex];
    if (anim >= spr->animCount)
        return NULL;
    return &spr->frames[frame + spr->animations[anim].frameListOffset];
}

inline Hitbox *GetHitbox(Animator *data, byte hitboxID)
{
    if (data && data->framePtrs)
        return &data->framePtrs[data->frameID].hitboxes[hitboxID & (FRAMEHITBOX_COUNT - 1)];
    else
        return NULL;
}

inline short GetFrameID(Animator *data)
{
    if (data && data->framePtrs)
        return data->framePtrs[data->frameID].id;
    else
        return 0;
}

void ProcessAnimation(Animator *data);

inline void SetSpriteAnimation(ushort spriteIndex, ushort animationID, Animator *data, bool32 forceApply, ushort frameID)
{
    if (spriteIndex >= SPRFILE_COUNT) {
        if (data)
            data->framePtrs = NULL;
        return;
    }
    if (!data)
        return;
    SpriteAnimation *spr = &spriteAnimationList[spriteIndex];
    if (animationID >= spr->animCount)
        return;

    SpriteAnimationEntry *anim = &spr->animations[animationID];
    SpriteFrame *frames        = &spr->frames[anim->frameListOffset];
    if (data->framePtrs == frames && !forceApply)
        return;

    data->framePtrs       = frames;
    data->animationTimer  = 0;
    data->frameID         = frameID;
    data->frameCount      = anim->frameCount;
    data->frameDelay      = data->framePtrs[frameID].delay;
    data->animationSpeed  = anim->animationSpeed;
    data->rotationFlag    = anim->rotationFlag;
    data->loopIndex       = anim->loopIndex;
    data->prevAnimationID = data->animationID;
    data->animationID     = animationID;
}

inline void EditAnimation(ushort spriteIndex, ushort animID, const char *name, int frameOffset, ushort frameCount, short animSpeed, byte loopIndex,
                   byte rotationFlag)
{
    if (spriteIndex < SPRFILE_COUNT) {
        SpriteAnimation *spr = &spriteAnimationList[spriteIndex];
        if (animID < spr->animCount) {
            SpriteAnimationEntry *anim = &spr->animations[animID];
            strcpy(hashBuffer, name);
            GenerateHash(anim->hash, strlen(hashBuffer));
            anim->frameListOffset = frameOffset;
            anim->frameCount      = frameCount;
            anim->animationSpeed  = animSpeed;
            anim->loopIndex       = loopIndex;
            anim->rotationFlag    = rotationFlag;
        }
    }
}

int GetStringWidth(ushort sprIndex, ushort animID, TextInfo *info, int startIndex, int length, int spacing);
void SetSpriteString(ushort spriteIndex, ushort animID, TextInfo *info);

#endif
