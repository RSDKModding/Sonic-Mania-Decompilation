#ifndef OBJ_CANISTA_H
#define OBJ_CANISTA_H

#include "SonicMania.h"

// Object Class
struct ObjectCanista {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox4;
    uint16 aniFrames;
    uint16 sfxPon;
};

// Entity Class
struct EntityCanista {
    RSDK_ENTITY
    StateMachine(state);
    uint8 triggerMode;
    Vector2 detectOffset;
    Vector2 detectSize;
    int32 shootOffset;
    int32 shootSize;
    int32 timer3;
    int32 timer2;
    Entity *detectedPlayer;
    Vector2 startPos;
    uint8 startDir;
    uint8 field_8D;
    uint8 timer;
    Hitbox hitbox;
    int32 field_98;
    int32 field_9C;
    Animator animator1;
    Animator animator2;
    Animator animator3;
};

// Object Struct
extern ObjectCanista *Canista;

// Standard Entity Events
void Canista_Update(void);
void Canista_LateUpdate(void);
void Canista_StaticUpdate(void);
void Canista_Draw(void);
void Canista_Create(void* data);
void Canista_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Canista_EditorDraw(void);
void Canista_EditorLoad(void);
#endif
void Canista_Serialize(void);

// Extra Entity Functions
void Canista_DebugSpawn(void);
void Canista_DebugDraw(void);

void Canista_CheckPlayerCollisions(void);
void Canista_CheckOnScreen(void);

void Canista_State_Setup(void);
void Canista_State_Unknown1(void);
void Canista_State_Unknown2(void);

void Canista_CheckPlayerCollisions2(void);
void Canista_State2_Unknown1(void);
void Canista_State2_Unknown2(void);

#endif //!OBJ_CANISTA_H
