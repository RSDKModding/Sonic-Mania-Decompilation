#ifndef OBJ_CACTULA_H
#define OBJ_CACTULA_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 sfxCactDrop;
} ObjectCactula;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 field_5C;
    int32 offsetY;
    bool32 flag;
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntityCactula;

// Object Struct
extern ObjectCactula *Cactula;

// Standard Entity Events
void Cactula_Update(void);
void Cactula_LateUpdate(void);
void Cactula_StaticUpdate(void);
void Cactula_Draw(void);
void Cactula_Create(void* data);
void Cactula_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Cactula_EditorDraw(void);
void Cactula_EditorLoad(void);
#endif
void Cactula_Serialize(void);

// Extra Entity Functions
void Cactula_DebugSpawn(void);
void Cactula_DebugDraw(void);

void Cactula_CheckPlayerCollisions(void);
bool32 Cactula_CheckCB(void);

void Cactula_State_Unknown1(void);
void Cactula_State_Unknown2(void);
void Cactula_State_Unknown3(void);

#endif //!OBJ_CACTULA_H
