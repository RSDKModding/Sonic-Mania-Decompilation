#ifndef OBJ_LRZSPIKEBALL_H
#define OBJ_LRZSPIKEBALL_H

#include "SonicMania.h"

// Object Class
struct ObjectLRZSpikeBall {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxCharge;
    uint16 sfxExplosion;
};

// Entity Class
struct EntityLRZSpikeBall {
    RSDK_ENTITY
    StateMachine(state);
    int32 interval;
    int32 intervalOffset;
    int32 strength;
    int32 field_68;
    Vector2 startPos;
    Vector2 offset;
    int32 timer;
    Hitbox hitboxBase;
    Hitbox hitboxBall;
    Animator animator1;
    Animator animator2;
};

// Object Struct
extern ObjectLRZSpikeBall *LRZSpikeBall;

// Standard Entity Events
void LRZSpikeBall_Update(void);
void LRZSpikeBall_LateUpdate(void);
void LRZSpikeBall_StaticUpdate(void);
void LRZSpikeBall_Draw(void);
void LRZSpikeBall_Create(void* data);
void LRZSpikeBall_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LRZSpikeBall_EditorDraw(void);
void LRZSpikeBall_EditorLoad(void);
#endif
void LRZSpikeBall_Serialize(void);

// Extra Entity Functions
int LRZSpikeBall_GetFrameID(void);

void LRZSpikeBall_CheckPlayerBaseCollisions(void);
void LRZSpikeBall_CheckPlayerBallCollisions(void);

void LRZSpikeBall_State_Setup(void);
void LRZSpikeBall_State_Unknown1(void);
void LRZSpikeBall_State_Unknown2(void);
void LRZSpikeBall_State_Unknown3(void);

#endif //!OBJ_LRZSPIKEBALL_H
