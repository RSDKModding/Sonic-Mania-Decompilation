#ifndef OBJ_PARALLAXSPRITE_H
#define OBJ_PARALLAXSPRITE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
} ObjectParallaxSprite;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Animator animator;
    byte aniID;
    byte attribute;
    Vector2 parallaxFactor;
    Vector2 scrollSpeed;
    Vector2 loopPoint;
    bool32 hiddenAtStart;
    Vector2 unknownPosA;
    Vector2 unknownPosB;
    Vector2 scrollPos;
    int sprX;
    int field_B0;
    int field_B4;
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
void ParallaxSprite_EditorDraw(void);
void ParallaxSprite_EditorLoad(void);
void ParallaxSprite_Serialize(void);

// Extra Entity Functions
void ParallaxSprite_Unknown1(void);
void ParallaxSprite_CreateCopy(void);
void ParallaxSprite_Unknown3(void);
void ParallaxSprite_Unknown4(void);
void ParallaxSprite_Unknown5(void);

#endif //!OBJ_PARALLAXSPRITE_H
