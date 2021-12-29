#ifndef OBJ_BATBOT_H
#define OBJ_BATBOT_H

#include "SonicMania.h"

// Object Class
struct ObjectBatbot {
    RSDK_OBJECT
    Hitbox hitbox;
    Hitbox spinbox;
    Hitbox attackbox;
    Hitbox hitboxPlayer;
    uint16 aniFrames;
    uint8 dirFlag[5];
};

// Entity Class
struct EntityBatbot {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    uint8 startDir;
    uint16 timer;
    int32 arcAngle;
    int32 startY;
    Vector2 landPos;
    int32 unused1;
    int32 unused2;
    int32 swoopAngle;
    Entity *playerPtr;
    uint8 lastDir;
    bool32 showWings;
    uint16 unused3; // set to 0, not used, also happens to be unused in S2
    Animator animator1;
    Animator animator2;
};

// Object Entity
extern ObjectBatbot *Batbot;

// Standard Entity Events
void Batbot_Update(void);
void Batbot_LateUpdate(void);
void Batbot_StaticUpdate(void);
void Batbot_Draw(void);
void Batbot_Create(void* data);
void Batbot_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Batbot_EditorDraw(void);
void Batbot_EditorLoad(void);
#endif
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
