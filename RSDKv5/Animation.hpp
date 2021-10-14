#ifndef ANIMATION_H
#define ANIMATION_H

#define SPRFILE_COUNT (0x400)

#define FRAMEHITBOX_COUNT (0x8)

enum RotationFlags { ROTFLAG_NONE, ROTFLAG_FULL, ROTFLAG_45DEG, ROTFLAG_90DEG, ROTFLAG_180DEG, ROTFLAG_STATICFRAMES };

struct SpriteAnimationEntry {
    RETRO_HASH(hash);
    int32 frameListOffset;
    uint16 frameCount;
    int16 animationSpeed;
    uint8 loopIndex;
    uint8 rotationFlag;
};

struct Hitbox {
    int16 left;
    int16 top;
    int16 right;
    int16 bottom;
};

struct SpriteFrame {
    int16 sprX;
    int16 sprY;
    int16 width;
    int16 height;
    int16 pivotX;
    int16 pivotY;
    ushort delay;
    uint16 id;
    uint8 sheetID;
    uint8 hitboxCnt;
    Hitbox hitboxes[FRAMEHITBOX_COUNT];
};

struct SpriteAnimation {
    RETRO_HASH(hash);
    SpriteFrame *frames;
    SpriteAnimationEntry *animations;
    uint16 animCount;
    uint8 scope;
};

struct Animator {
    SpriteFrame *framePtrs;
    int32 frameID;
    int16 animationID;
    int16 prevAnimationID;
    int16 animationSpeed;
    int16 animationTimer;
    int16 frameDelay;
    int16 frameCount;
    uint8 loopIndex;
    uint8 rotationFlag;
};

extern SpriteAnimation spriteAnimationList[SPRFILE_COUNT];

short LoadSpriteAnimation(const char *filename, Scopes scope);
short CreateSpriteAnimation(const char *filename, uint frameCount, uint animCount, Scopes scope);

inline ushort GetSpriteAnimation(ushort sprIndex, const char *name)
{
    if (sprIndex >= SPRFILE_COUNT)
        return 0;
    SpriteAnimation *spr = &spriteAnimationList[sprIndex];

    RETRO_HASH(hash);
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

inline Hitbox *GetHitbox(Animator *animator, byte hitboxID)
{
    if (animator && animator->framePtrs)
        return &animator->framePtrs[animator->frameID].hitboxes[hitboxID & (FRAMEHITBOX_COUNT - 1)];
    else
        return NULL;
}

inline short GetFrameID(Animator *animator)
{
    if (animator && animator->framePtrs)
        return animator->framePtrs[animator->frameID].id;
    else
        return 0;
}

void ProcessAnimation(Animator *animator);

inline void SetSpriteAnimation(ushort spriteIndex, ushort animationID, Animator *animator, bool32 forceApply, short frameID)
{
    if (spriteIndex >= SPRFILE_COUNT) {
        if (animator)
            animator->framePtrs = NULL;
        return;
    }
    if (!animator)
        return;
    SpriteAnimation *spr = &spriteAnimationList[spriteIndex];
    if (animationID >= spr->animCount)
        return;

    SpriteAnimationEntry *anim = &spr->animations[animationID];
    SpriteFrame *frames        = &spr->frames[anim->frameListOffset];
    if (animator->framePtrs == frames && !forceApply)
        return;

    animator->framePtrs       = frames;
    animator->animationTimer  = 0;
    animator->frameID         = frameID;
    animator->frameCount      = anim->frameCount;
    animator->frameDelay      = animator->framePtrs[frameID].delay;
    animator->animationSpeed  = anim->animationSpeed;
    animator->rotationFlag    = anim->rotationFlag;
    animator->loopIndex       = anim->loopIndex;
    animator->prevAnimationID = animator->animationID;
    animator->animationID     = animationID;
}

inline void EditSpriteAnimation(ushort spriteIndex, ushort animID, const char *name, int frameOffset, ushort frameCount, short animSpeed,
                                byte loopIndex, byte rotationFlag)
{
    if (spriteIndex < SPRFILE_COUNT) {
        SpriteAnimation *spr = &spriteAnimationList[spriteIndex];
        if (animID < spr->animCount) {
            SpriteAnimationEntry *anim = &spr->animations[animID];
            GEN_HASH(name, anim->hash);
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
