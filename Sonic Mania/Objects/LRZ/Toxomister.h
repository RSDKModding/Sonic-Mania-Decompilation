#ifndef OBJ_TOXOMISTER_H
#define OBJ_TOXOMISTER_H

#include "SonicMania.h"

// Object Class
struct ObjectToxomister {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint16 aniFrames;
};

// Entity Class
struct EntityToxomister {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    uint8 startDir;
    Entity *link;
    EntityPlayer *grabbedPlayer;
    int32 timer;
    uint16 shakeTimer;
    uint16 shakeCount;
    uint16 prevShakeFlags;
    Animator animator;
};

// Object Struct
extern ObjectToxomister *Toxomister;

// Standard Entity Events
void Toxomister_Update(void);
void Toxomister_LateUpdate(void);
void Toxomister_StaticUpdate(void);
void Toxomister_Draw(void);
void Toxomister_Create(void* data);
void Toxomister_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Toxomister_EditorDraw(void);
void Toxomister_EditorLoad(void);
#endif
void Toxomister_Serialize(void);

// Extra Entity Functions
void Toxomister_DebugSpawn(void);
void Toxomister_DebugDraw(void);

void Toxomister_CheckPlayerCollisions(void);
void Toxomister_CheckOnScreen(void);
void Toxomister_CheckMistOnScreen(void);
void Toxomister_CheckPlayerMistCollisions(void);

void Toxomister_State_Setup(void);
void Toxomister_State_Unknown1(void);

void Toxomister_State1_Unknown1(void);
void Toxomister_State1_Unknown2(void);
void Toxomister_State1_Unknown3(void);
void Toxomister_State1_Unknown4(void);
void Toxomister_State1_Unknown5(void);

#endif //!OBJ_TOXOMISTER_H
