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
typedef struct {
    RSDK_OBJECT
    Hitbox hitboxHurt;
    Hitbox hitboxExplode;
    Hitbox hitboxShrapnel;
    ushort aniFrames;
    ushort sfxExplosion;
    ushort sfxButton;
    ushort sfxPon;
} ObjectMatryoshkaBom;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    PlaneFilterTypes planeFilter;
    MatryoshkaSizes size;
    bool32 canExplode;
    bool32 destroyOffscreen;
    int offsetY;
    int offsetX;
    Vector2 startPos;
    byte startDir;
    int timer;
    int field_84;
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntityMatryoshkaBom;

// Object Struct
extern ObjectMatryoshkaBom *MatryoshkaBom;

// Standard Entity Events
void MatryoshkaBom_Update(void);
void MatryoshkaBom_LateUpdate(void);
void MatryoshkaBom_StaticUpdate(void);
void MatryoshkaBom_Draw(void);
void MatryoshkaBom_Create(void* data);
void MatryoshkaBom_StageLoad(void);
void MatryoshkaBom_EditorDraw(void);
void MatryoshkaBom_EditorLoad(void);
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
