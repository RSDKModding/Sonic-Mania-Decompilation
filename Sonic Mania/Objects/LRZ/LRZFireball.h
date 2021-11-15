#ifndef OBJ_LRZFIREBALL_H
#define OBJ_LRZFIREBALL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
    uint16 sfxFireball;
} ObjectLRZFireball;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Animator animator;
    uint8 type;
    uint16 interval;
    uint16 intervalOffset;
    int32 field_80;
    int32 field_84;
    int32 field_88;
} EntityLRZFireball;

// Object Struct
extern ObjectLRZFireball *LRZFireball;

// Standard Entity Events
void LRZFireball_Update(void);
void LRZFireball_LateUpdate(void);
void LRZFireball_StaticUpdate(void);
void LRZFireball_Draw(void);
void LRZFireball_Create(void* data);
void LRZFireball_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LRZFireball_EditorDraw(void);
void LRZFireball_EditorLoad(void);
#endif
void LRZFireball_Serialize(void);

// Extra Entity Functions
void LRZFireball_CheckPlayerCollisions(void);
void LRZFireball_CheckOnScreen(void);
void LRZFireball_CheckTileCollisions(void);

void LRZFireball_State_Type0(void);
void LRZFireball_State_Type1(void);
void LRZFireball_State_Type2(void);

void LRZFireball_StateChild_Type0(void);
void LRZFireball_StateChild_Type1(void);
void LRZFireball_StateChild_Type2(void);

void LRZFireball_StateDraw_Visible(void);

#endif //!OBJ_LRZFIREBALL_H
