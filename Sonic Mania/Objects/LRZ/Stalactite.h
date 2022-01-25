#ifndef OBJ_STALACTITE_H
#define OBJ_STALACTITE_H

#include "SonicMania.h"

// Object Class
struct ObjectStalactite {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint16 aniFrames;
    uint16 sfxShoot;
};

// Entity Class
struct EntityStalactite {
    RSDK_ENTITY
    StateMachine(state);
    Animator animator;
};

// Object Struct
extern ObjectStalactite *Stalactite;

// Standard Entity Events
void Stalactite_Update(void);
void Stalactite_LateUpdate(void);
void Stalactite_StaticUpdate(void);
void Stalactite_Draw(void);
void Stalactite_Create(void* data);
void Stalactite_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Stalactite_EditorDraw(void);
void Stalactite_EditorLoad(void);
#endif
void Stalactite_Serialize(void);

// Extra Entity Functions
void Stalactite_State_Unknown1(void);
void Stalactite_State_Unknown2(void);
void Stalactite_State_Unknown3(void);
void Stalactite_State_Unknown4(void);
void Stalactite_State_Unknown5(void);

#endif //!OBJ_STALACTITE_H
