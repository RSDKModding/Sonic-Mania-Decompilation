#ifndef OBJ_MATRYOSHKABOM_H
#define OBJ_MATRYOSHKABOM_H

#include "SonicMania.h"

typedef enum {
    MATRYOSHKA_SIZE_BIG,
    MATRYOSHKA_SIZE_MED,
    MATRYOSHKA_SIZE_SMALL,
    MATRYOSHKA_SIZE_SHRAPNEL,
} MatryoshkaSizes;

// Object Class
struct ObjectMatryoshkaBom {
    RSDK_OBJECT
    Hitbox hitboxHurt;
    Hitbox hitboxExplode;
    Hitbox hitboxShrapnel;
    uint16 aniFrames;
    uint16 sfxExplosion;
    uint16 sfxButton;
    uint16 sfxPon;
};

// Entity Class
struct EntityMatryoshkaBom {
    RSDK_ENTITY
    StateMachine(state);
    PlaneFilterTypes planeFilter;
    MatryoshkaSizes size;
    bool32 canExplode;
    bool32 destroyOffscreen;
    int32 offsetY;
    int32 offsetX;
    Vector2 startPos;
    uint8 startDir;
    int32 timer;
    int32 field_84;
    Animator animator1;
    Animator animator2;
    Animator animator3;
};

// Object Struct
extern ObjectMatryoshkaBom *MatryoshkaBom;

// Standard Entity Events
void MatryoshkaBom_Update(void);
void MatryoshkaBom_LateUpdate(void);
void MatryoshkaBom_StaticUpdate(void);
void MatryoshkaBom_Draw(void);
void MatryoshkaBom_Create(void* data);
void MatryoshkaBom_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void MatryoshkaBom_EditorDraw(void);
void MatryoshkaBom_EditorLoad(void);
#endif
void MatryoshkaBom_Serialize(void);

// Extra Entity Functions
void MatryoshkaBom_DebugSpawn(void);
void MatryoshkaBom_DebugDraw(void);

void MatryoshkaBom_CheckPlayerCollisions(void);
void MatryoshkaBom_CheckOnScreen(void);

void MatryoshkaBom_State_Setup(void);
void MatryoshkaBom_Unknown5(void);
void MatryoshkaBom_Unknown6(void);
void MatryoshkaBom_Unknown7(void);
void MatryoshkaBom_Unknown8(void);
void MatryoshkaBom_State_Exploding(void);
void MatryoshkaBom_Unknown10(void);
void MatryoshkaBom_Unknown11(void);

#endif //!OBJ_MATRYOSHKABOM_H
