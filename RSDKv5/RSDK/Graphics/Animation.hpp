#ifndef ANIMATION_H
#define ANIMATION_H

namespace RSDK
{

#define SPRFILE_COUNT     (0x400)
#define SPRITEFRAME_COUNT (0x400)
#define SPRITEANIM_COUNT  (0x40)

#define FRAMEHITBOX_COUNT (0x8)

#define RSDK_SIGNATURE_SPR (0x525053) // "SPR"

enum RotationSyles { ROTSTYLE_NONE, ROTSTYLE_FULL, ROTFLAG_45DEG, ROTSTYLE_90DEG, ROTSTYLE_180DEG, ROTSTYLE_STATICFRAMES };

struct Hitbox {
    int16 left;
    int16 top;
    int16 right;
    int16 bottom;
};

// this is all the data the game can access
typedef struct {
    int16 sprX;
    int16 sprY;
    int16 width;
    int16 height;
    int16 pivotX;
    int16 pivotY;
    uint16 duration;
    uint16 unicodeChar;
    uint8 sheetID;
} GameSpriteFrameType;

static GameSpriteFrameType GameSpriteFrame;

// hitboxCount & hitboxes are "private"
// the proof of this is that "GetHitbox(animator, uint8 hitboxID)" exists, which means that Animator::frames must be a void*
// otherwise you could just do animator->frames[x].hitboxes[y]
// further proof of this is SVAR_SPRITEFRAME, to which the ONLY matching in the entire engine struct is "SpriteFrame" without the hitbox stuff.
struct SpriteFrame : public GameSpriteFrameType {
    typedef decltype(GameSpriteFrame) frame;

    uint8 hitboxCount;
    Hitbox hitboxes[FRAMEHITBOX_COUNT];
};

struct SpriteAnimationEntry {
    RETRO_HASH_MD5(hash);
    int32 frameListOffset;
    uint16 frameCount;
    int16 animationSpeed;
    uint8 loopIndex;
    uint8 rotationStyle;
};

struct SpriteAnimation {
    RETRO_HASH_MD5(hash);
    SpriteFrame *frames;
    SpriteAnimationEntry *animations;
    uint16 animCount;
    uint8 scope;
};

struct Animator {
    SpriteFrame *frames;
    int32 frameID;
    int16 animationID;
    int16 prevAnimationID;
    int16 speed;
    int16 timer;
    int16 frameDuration;
    int16 frameCount;
    uint8 loopIndex;
    uint8 rotationStyle;
};

extern SpriteAnimation spriteAnimationList[SPRFILE_COUNT];

uint16 LoadSpriteAnimation(const char *filename, int32 scope);
uint16 CreateSpriteAnimation(const char *filename, uint32 frameCount, uint32 animCount, int32 scope);

inline uint16 FindSpriteAnimation(uint16 aniFrames, const char *name)
{
    if (aniFrames >= SPRFILE_COUNT)
        return 0;

    SpriteAnimation *spr = &spriteAnimationList[aniFrames];

    RETRO_HASH_MD5(hash);
    GEN_HASH_MD5(name, hash);

    for (int32 a = 0; a < spr->animCount; ++a) {
        if (HASH_MATCH_MD5(hash, spr->animations[a].hash))
            return a;
    }

    return -1;
}

inline SpriteFrame *GetFrame(uint16 aniFrames, uint16 anim, int32 frame)
{
    if (aniFrames >= SPRFILE_COUNT)
        return NULL;

    SpriteAnimation *spr = &spriteAnimationList[aniFrames];
    if (anim >= spr->animCount)
        return NULL;

    return &spr->frames[frame + spr->animations[anim].frameListOffset];
}

inline Hitbox *GetHitbox(Animator *animator, uint8 hitboxID)
{
    if (animator && animator->frames)
        return &animator->frames[animator->frameID].hitboxes[hitboxID & (FRAMEHITBOX_COUNT - 1)];
    else
        return NULL;
}

inline int16 GetFrameID(Animator *animator)
{
    if (animator && animator->frames)
        return animator->frames[animator->frameID].unicodeChar;

    return 0;
}

void ProcessAnimation(Animator *animator);

inline void SetSpriteAnimation(uint16 aniFrames, uint16 animationID, Animator *animator, bool32 forceApply, int16 frameID)
{
    if (aniFrames >= SPRFILE_COUNT || !animator) {
        if (animator)
            animator->frames = NULL;
        return;
    }

    SpriteAnimation *spr = &spriteAnimationList[aniFrames];
    if (animationID >= spr->animCount)
        return;

    SpriteAnimationEntry *anim = &spr->animations[animationID];
    SpriteFrame *frames        = &spr->frames[anim->frameListOffset];
    if (animator->frames == frames && !forceApply)
        return;

    animator->frames          = frames;
    animator->timer           = 0;
    animator->frameID         = frameID;
    animator->frameCount      = anim->frameCount;
    animator->frameDuration   = animator->frames[frameID].duration;
    animator->speed           = anim->animationSpeed;
    animator->rotationStyle   = anim->rotationStyle;
    animator->loopIndex       = anim->loopIndex;
    animator->prevAnimationID = animator->animationID;
    animator->animationID     = animationID;
}

inline void EditSpriteAnimation(uint16 aniFrames, uint16 animID, const char *name, int32 frameOffset, uint16 frameCount, int16 animSpeed,
                                uint8 loopIndex, uint8 rotationStyle)
{
    if (aniFrames < SPRFILE_COUNT) {
        SpriteAnimation *spr = &spriteAnimationList[aniFrames];
        if (animID < spr->animCount) {
            SpriteAnimationEntry *anim = &spr->animations[animID];
            GEN_HASH_MD5(name, anim->hash);
            anim->frameListOffset = frameOffset;
            anim->frameCount      = frameCount;
            anim->animationSpeed  = animSpeed;
            anim->loopIndex       = loopIndex;
            anim->rotationStyle   = rotationStyle;
        }
    }
}

int32 GetStringWidth(uint16 aniFrames, uint16 animID, String *string, int32 startIndex, int32 length, int32 spacing);
void SetSpriteString(uint16 aniFrames, uint16 animID, String *string);

inline void ClearSpriteAnimations()
{
    // Unload animations
    for (int32 s = 0; s < SPRFILE_COUNT; ++s) {
        if (spriteAnimationList[s].scope != SCOPE_GLOBAL) {
            MEM_ZERO(spriteAnimationList[s]);
            spriteAnimationList[s].scope = SCOPE_NONE;
        }
    }
}

} // namespace RSDK

#endif
