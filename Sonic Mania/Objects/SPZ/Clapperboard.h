#ifndef OBJ_CLAPPERBOARD_H
#define OBJ_CLAPPERBOARD_H

#include "SonicMania.h"

// Object Class
struct ObjectClapperboard {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxWhack;
};

// Entity Class
struct EntityClapperboard {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateCollide);
    uint8 takeID;
    int32 field_64;
    int32 field_68;
    Hitbox hitbox;
    int32 dword74;
    int32 dword78;
    int32 dword7C;
    int32 dword80;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
};

// Object Struct
extern ObjectClapperboard *Clapperboard;

// Standard Entity Events
void Clapperboard_Update(void);
void Clapperboard_LateUpdate(void);
void Clapperboard_StaticUpdate(void);
void Clapperboard_Draw(void);
void Clapperboard_Create(void* data);
void Clapperboard_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Clapperboard_EditorDraw(void);
void Clapperboard_EditorLoad(void);
#endif
void Clapperboard_Serialize(void);

// Extra Entity Functions
void Clapperboard_StateCollide_NoFlip(void);
void Clapperboard_StateCollide_FlipX(void);

void Clapperboard_State_Unknown1(void);
void Clapperboard_State_Unknown2(void);
void Clapperboard_State_Unknown3(void);
void Clapperboard_State_Unknown4(void);
void Clapperboard_State_Unknown5(void);

#endif //!OBJ_CLAPPERBOARD_H
