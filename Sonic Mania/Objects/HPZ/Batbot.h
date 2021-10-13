#ifndef OBJ_BATBOT_H
#define OBJ_BATBOT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitboxPlayer;
    uint16 aniFrames;
    uint8 dirFlag[5];
} ObjectBatbot;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    uint8 startDir;
    uint16 timer;
    int field_68;
    int field_6C;
    Vector2 posUnknown;
    int field_78;
    int field_7C;
    int swoopAngle;
    Entity *playerPtr;
    uint8 field_88;
    bool32 flag;
    uint16 field_90;
    Animator animator1;
    Animator animator2;
} EntityBatbot;

// Object Entity
extern ObjectBatbot *Batbot;

// Standard Entity Events
void Batbot_Update(void);
void Batbot_LateUpdate(void);
void Batbot_StaticUpdate(void);
void Batbot_Draw(void);
void Batbot_Create(void* data);
void Batbot_StageLoad(void);
void Batbot_EditorDraw(void);
void Batbot_EditorLoad(void);
void Batbot_Serialize(void);

// Extra Entity Functions
void Batbot_DebugSpawn(void);
void Batbot_DebugDraw(void);

void Batbot_HandlePlayerInteractions(void);

void Batbot_State_Setup(void);
void Batbot_State_Idle(void);
void Batbot_State_Attack(void);
void Batbot_State_SwoopRight(void);
void Batbot_State_SwoopLeft(void);

#endif //!OBJ_BATBOT_H
