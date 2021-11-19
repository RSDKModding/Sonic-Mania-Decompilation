#ifndef OBJ_PARALLAXSPRITE_H
#define OBJ_PARALLAXSPRITE_H

#include "SonicMania.h"

typedef enum {
    PSPRITE_ATTR_NONE,
    PSPRITE_ATTR_1,
    PSPRITE_ATTR_2,
    PSPRITE_ATTR_SPAWNER,
    PSPRITE_ATTR_FADEOUT,
    PSPRITE_ATTR_BLENDHIGH,
    PSPRITE_ATTR_SHIFT,
} ParallaxSpriteAttributes;

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectParallaxSprite;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Animator animator;
    uint8 aniID;
    uint8 attribute;
    Vector2 parallaxFactor;
    Vector2 scrollSpeed;
    Vector2 loopPoint;
    bool32 hiddenAtStart;
    colour colour1;
    colour colour2;
    Vector2 acceleration;
    Vector2 scrollPos;
    int32 sprX;
    int32 timerSpeed;
    int32 xSpeed;
} EntityParallaxSprite;

// Object Struct
extern ObjectParallaxSprite *ParallaxSprite;

// Standard Entity Events
void ParallaxSprite_Update(void);
void ParallaxSprite_LateUpdate(void);
void ParallaxSprite_StaticUpdate(void);
void ParallaxSprite_Draw(void);
void ParallaxSprite_Create(void* data);
void ParallaxSprite_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void ParallaxSprite_EditorDraw(void);
void ParallaxSprite_EditorLoad(void);
#endif
void ParallaxSprite_Serialize(void);

// Extra Entity Functions
void ParallaxSprite_State_RotateAndScroll(void);
void ParallaxSprite_State_Spawner(void);
void ParallaxSprite_State_FadeOutAndDie(void);
void ParallaxSprite_State_FadeIntoHalf(void);
void ParallaxSprite_State_FadeOut(void);

#endif //!OBJ_PARALLAXSPRITE_H
