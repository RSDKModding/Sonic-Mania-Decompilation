#ifndef OBJ_BUGGERNAUT_H
#define OBJ_BUGGERNAUT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitbox2;
    uint16 aniFrames;
} ObjectBuggernaut;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    bool32 passThrough;
    int32 timer;
    int32 timer2;
    Vector2 startPos;
    Entity *parent;
    Animator animator1;
    Animator animator2;
} EntityBuggernaut;

// Object Struct
extern ObjectBuggernaut *Buggernaut;

// Standard Entity Events
void Buggernaut_Update(void);
void Buggernaut_LateUpdate(void);
void Buggernaut_StaticUpdate(void);
void Buggernaut_Draw(void);
void Buggernaut_Create(void* data);
void Buggernaut_StageLoad(void);
void Buggernaut_EditorDraw(void);
void Buggernaut_EditorLoad(void);
void Buggernaut_Serialize(void);

// Extra Entity Functions
void Buggernaut_DebugSpawn(void);
void Buggernaut_DebugDraw(void);

void Buggernaut_CheckPlayerCollisions(void);
void Buggernaut_CheckOnScreen(void);
bool32 Buggernaut_HandleTileCollisionsX(void);
bool32 Buggernaut_HandleTileCollisionsY(void);

void Buggernaut_State_Setup(void);
void Buggernaut_State_Unknown1(void);
void Buggernaut_State_Unknown2(void);
void Buggernaut_State_Unknown3(void);

void Buggernaut_State2_Unknown(void);

#endif //!OBJ_BUGGERNAUT_H
