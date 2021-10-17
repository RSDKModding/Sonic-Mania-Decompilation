#ifndef OBJ_MSBOMB_H
#define OBJ_MSBOMB_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxExplosion;
} ObjectMSBomb;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int field_5C;
    int timer;
    Hitbox hitbox;
    Animator animator;
} EntityMSBomb;

// Object Struct
extern ObjectMSBomb *MSBomb;

// Standard Entity Events
void MSBomb_Update(void);
void MSBomb_LateUpdate(void);
void MSBomb_StaticUpdate(void);
void MSBomb_Draw(void);
void MSBomb_Create(void* data);
void MSBomb_StageLoad(void);
void MSBomb_EditorDraw(void);
void MSBomb_EditorLoad(void);
void MSBomb_Serialize(void);

// Extra Entity Functions
void MSBomb_Unknown1(void);
void MSBomb_Unknown2(void);
void MSBomb_Unknown3(void);
void MSBomb_Unknown4(void);

#endif

#endif //!OBJ_MSBOMB_H
