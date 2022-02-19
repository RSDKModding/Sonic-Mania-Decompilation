#ifndef OBJ_HOTARU_H
#define OBJ_HOTARU_H

#include "SonicMania.h"

// Object Class
struct ObjectHotaru {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    int32 field_14;
    int32 field_18;
    Hitbox hitbox3;
    uint16 aniFrames;
};

// Entity Class
struct EntityHotaru {
    RSDK_ENTITY
    StateMachine(state);
    uint8 quality;
    Vector2 dist;
    int32 distX1;
    int32 distX2;
    int32 speed;
    uint8 preset;
    Vector2 offset;
    Vector2 dist2;
    uint8 field_88;
    uint8 flags;
    Vector2 startPos;
    uint8 startDir;
    uint8 screenID;
    int32 timer;
    Entity *playerPtr;
    uint8 field_A0;
    uint8 field_A1;
    uint8 field_A2;
    Vector2 offset2;
    Animator animator1;
    Animator animator2;
    Animator animator3;
};

// Object Struct
extern ObjectHotaru *Hotaru;

// Standard Entity Events
void Hotaru_Update(void);
void Hotaru_LateUpdate(void);
void Hotaru_StaticUpdate(void);
void Hotaru_Draw(void);
void Hotaru_Create(void* data);
void Hotaru_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Hotaru_EditorDraw(void);
void Hotaru_EditorLoad(void);
#endif
void Hotaru_Serialize(void);

// Extra Entity Functions
void Hotaru_DebugSpawn(void);
void Hotaru_DebugDraw(void);

void Hotaru_CheckPlayerCollisions(void);
void Hotaru_CheckOffScreen(void);

void Hotaru_State_Setup(void);
void Hotaru_State_Unknown1(void);
void Hotaru_State_Unknown2(void);
void Hotaru_State_Unknown3(void);
void Hotaru_State_Unknown4(void);
void Hotaru_State_Unknown5(void);
void Hotaru_State_Unknown6(void);
void Hotaru_State_Unknown7(void);

#endif //!OBJ_HOTARU_H
