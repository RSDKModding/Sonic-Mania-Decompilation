#ifndef OBJ_PARALLAXSPRITE_H
#define OBJ_PARALLAXSPRITE_H

#include "Game.h"

typedef enum {
    PARALLAXSPRITE_ATTR_STANDARD,
    PARALLAXSPRITE_ATTR_WINDMILL,
    PARALLAXSPRITE_ATTR_COLORS,
    PARALLAXSPRITE_ATTR_EMITTER,
    PARALLAXSPRITE_ATTR_PARTICLE,
    PARALLAXSPRITE_ATTR_BLENDHIGH,
    PARALLAXSPRITE_ATTR_SHIFT,
} ParallaxSpriteAttributes;

typedef enum {
    PARALLAXSPRITE_NONE,
} ParallaxSpriteAniIDs;

// Object Class
struct ObjectParallaxSprite {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityParallaxSprite {
    RSDK_ENTITY
    StateMachine(state);
    Animator animator;
    uint8 aniID;
    uint8 attribute;
    Vector2 parallaxFactor;
    Vector2 scrollSpeed;
    Vector2 loopPoint;
    bool32 hiddenAtStart;
    color color1;
    color color2;
    Vector2 acceleration;
    Vector2 scrollPos;
    int32 sprX;
    int32 timerSpeed;
    int32 xSpeed;
};

// Object Struct
extern ObjectParallaxSprite *ParallaxSprite;

// Standard Entity Events
void ParallaxSprite_Update(void);
void ParallaxSprite_LateUpdate(void);
void ParallaxSprite_StaticUpdate(void);
void ParallaxSprite_Draw(void);
void ParallaxSprite_Create(void *data);
void ParallaxSprite_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ParallaxSprite_EditorDraw(void);
void ParallaxSprite_EditorLoad(void);
#endif
void ParallaxSprite_Serialize(void);

// Extra Entity Functions
void ParallaxSprite_State_Normal(void);
void ParallaxSprite_State_Emitter(void);
void ParallaxSprite_State_Particle(void);
void ParallaxSprite_State_FadeIntoHalf(void);
void ParallaxSprite_State_FadeOut(void);

#endif //! OBJ_PARALLAXSPRITE_H
