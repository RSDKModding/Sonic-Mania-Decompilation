#ifndef ANIMATION_H
#define ANIMATION_H

#define SPRFILE_COUNT     (0x400)
#define ANIMATION_COUNT   (0x400)
#define SPRITEFRAME_COUNT (0x1000)

#define FRAMEHITBOX_COUNT (0x8)

enum AnimrotationFlags { ROTFLAG_NONE, ROTFLAG_FULL, ROTFLAG_45DEG, ROTFLAG_STATICFRAMES };

struct SpriteAnimation {
    byte scope;
    uint hash[4];
    ushort animCount;
    int aniListOffset;
};

struct SpriteAnimationEntry {
    uint hash[4];
    int frameListOffset;
    ushort frameCount;
    short animationSpeed;
    byte loopIndex;
    byte rotationFlag;
    byte field_1A;
    byte field_1B;
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

struct EntityAnimationData {
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

extern SpriteAnimation animationFileList[SPRFILE_COUNT];
extern SpriteFrame animFrames[SPRITEFRAME_COUNT];
extern int animFrameCount;
extern SpriteAnimationEntry animationList[ANIMATION_COUNT];
extern int animationCount;

short LoadAnimation(const char *filename, Scopes scope);

inline SpriteFrame *GetFrame(ushort sprIndex, ushort anim, int frame)
{
    if (sprIndex >= SPRFILE_COUNT)
        return NULL;
    SpriteAnimation *spr = &animationFileList[sprIndex];
    if (anim + spr->aniListOffset >= ANIMATION_COUNT)
        return NULL;
    return &animFrames[frame + animationList[anim + spr->aniListOffset].frameListOffset];
}

inline Hitbox *GetHitbox(EntityAnimationData *data, byte hitboxID)
{
    if (data && data->framePtrs)
        return &data->framePtrs[data->frameID].hitboxes[hitboxID & (FRAMEHITBOX_COUNT - 1)];
    else
        return NULL;
}

inline short GetFrameID(EntityAnimationData *data)
{
    if (data && data->framePtrs)
        return data->framePtrs[data->frameID].id;
    else
        return 0;
}

void ProcessAnimation(EntityAnimationData *data);

inline void SetSpriteAnimation(ushort spriteIndex, ushort animationID, EntityAnimationData *data, bool forceApply, ushort frameID)
{
    if (spriteIndex >= SPRFILE_COUNT) {
        if (data)
            data->framePtrs = NULL;
        return;
    }
    if (!data)
        return;
    if (animationID >= animationFileList[spriteIndex].animCount)
        return;

    SpriteAnimationEntry *anim = &animationList[animationFileList[spriteIndex].aniListOffset + animationID];
    SpriteFrame *frames        = &animFrames[anim->frameListOffset];
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

inline void SetModelAnimation(ushort modelAnim, EntityAnimationData *data, short animSpeed, byte loopIndex, bool forceApply, ushort frameID)
{
    if (modelAnim >= /*MODEL_MAX*/ 0x100) {
        if (data)
            data->framePtrs = 0;
        return;
    }
    if (!data)
        return;

    if (data->animationID == modelAnim && !forceApply)
        return;
    data->framePtrs       = (SpriteFrame *)1;
    data->animationTimer  = 0;
    data->frameID         = frameID;
    data->frameCount      = 0; // ModelFrameCount[22 * modelAnim];
    data->animationSpeed  = animSpeed;
    data->prevAnimationID = data->animationID;
    data->frameDelay      = 0x100;
    data->loopIndex       = loopIndex;
    data->animationID     = modelAnim;
}

#endif
