#ifndef OBJ_VULTRON_H
#define OBJ_VULTRON_H

#include "SonicMania.h"

// Object Class
struct ObjectVultron {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 sfxVultron;
};

// Entity Class
struct EntityVultron {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    Vector2 startPos;
    uint8 startDir;
    uint16 dist;
    uint16 distance;
    int32 storeY;
    Hitbox hitbox;
    Animator animator1;
    Animator animator2;
};

// Object Struct
extern ObjectVultron *Vultron;

// Standard Entity Events
void Vultron_Update(void);
void Vultron_LateUpdate(void);
void Vultron_StaticUpdate(void);
void Vultron_Draw(void);
void Vultron_Create(void* data);
void Vultron_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Vultron_EditorDraw(void);
void Vultron_EditorLoad(void);
#endif
void Vultron_Serialize(void);

// Extra Entity Functions
void Vultron_DebugSpawn(void);
void Vultron_DebugDraw(void);

void Vultron_CheckPlayerCollisions(void);
void Vultron_CheckOnScreen(void);

void Vultron_State_Setup(void);
void Vultron_State_Unknown1(void);
void Vultron_State_Unknown2(void);
void Vultron_State_Unknown3(void);
void Vultron_State_Unknown4(void);
void Vultron_State_Unknown5(void);
void Vultron_State_Unknown6(void);

void Vultron_State2_Unknown(void);

#endif //!OBJ_VULTRON_H
