#ifndef OBJ_SSZSPIKEBALL_H
#define OBJ_SSZSPIKEBALL_H

#include "SonicMania.h"

// Object Class
struct ObjectSSZSpikeBall {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxPon;
    Hitbox hitboxBall;
    Hitbox hitboxBase[4];
};

// Entity Class
struct EntitySSZSpikeBall {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    uint16 interval;
    uint16 intervalOffset;
    int32 speed;
    int32 dist;
    uint8 timer;
    Vector2 spikeBallPos;
    Animator animator1;
    Animator animator2;
};

// Object Struct
extern ObjectSSZSpikeBall *SSZSpikeBall;

// Standard Entity Events
void SSZSpikeBall_Update(void);
void SSZSpikeBall_LateUpdate(void);
void SSZSpikeBall_StaticUpdate(void);
void SSZSpikeBall_Draw(void);
void SSZSpikeBall_Create(void* data);
void SSZSpikeBall_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void SSZSpikeBall_EditorDraw(void);
void SSZSpikeBall_EditorLoad(void);
#endif
void SSZSpikeBall_Serialize(void);

// Extra Entity Functions
void SSZSpikeBall_State_Setup(void);
void SSZSpikeBall_Unknown2(void);
void SSZSpikeBall_Unknown3(void);
void SSZSpikeBall_Unknown4(void);
void SSZSpikeBall_Unknown5(void);
void SSZSpikeBall_Unknown6(void);
void SSZSpikeBall_Unknown7(void);
void SSZSpikeBall_Unknown8(void);

#endif //!OBJ_SSZSPIKEBALL_H
