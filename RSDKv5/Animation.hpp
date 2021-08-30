#ifndef ANIMATION_H
#define ANIMATION_H

#define SPRFILE_COUNT (0x400)

#define FRAMEHITBOX_COUNT (0x8)

enum RotationFlags { ROTFLAG_NONE, ROTFLAG_FULL, ROTFLAG_45DEG, ROTFLAG_90DEG, ROTFLAG_180DEG, ROTFLAG_STATICFRAMES };

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
    short sprX;
    short sprY;
    short width;
    short height;
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

short LoadSpriteAnimation(const char *filename, Scopes scope);
short CreateSpriteAnimation(const char *filename, uint frameCount, uint animCount, Scopes scope);

inline ushort GetSpriteAnimation(ushort sprIndex, const char *name)
{
    if (sprIndex >= SPRFILE_COUNT)
        return 0;
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
