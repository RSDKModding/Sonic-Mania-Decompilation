#ifndef OBJ_BLADEPOLE_H
#define OBJ_BLADEPOLE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox1;
    Hitbox hitbox2;
} ObjectBladePole;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint16 timer;
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntityBladePole;

// Object Struct
extern ObjectBladePole *BladePole;

// Standard Entity Events
void BladePole_Update(void);
void BladePole_LateUpdate(void);
void BladePole_StaticUpdate(void);
void BladePole_Draw(void);
void BladePole_Create(void* data);
void BladePole_StageLoad(void);
void BladePole_EditorDraw(void);
void BladePole_EditorLoad(void);
void BladePole_Serialize(void);

// Extra Entity Functions
void BladePole_DrawSprites(void);

bool32 BladePole_SetAnimation(Animator *animator);
void BladePole_CheckPlayerCollision(Hitbox *hitbox);

void BladePole_Unknown4(void);
void BladePole_Unknown5(void);

#endif //!OBJ_BLADEPOLE_H
