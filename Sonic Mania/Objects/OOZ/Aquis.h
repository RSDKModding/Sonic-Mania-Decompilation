#ifndef OBJ_AQUIS_H
#define OBJ_AQUIS_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    uint16 aniFrames;
    uint16 sfxShot;
} ObjectAquis;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
    uint8 field_60;
    int timer2;
    Vector2 startPos;
    uint8 startDir;
    Animator animator1;
    Animator animator2;
} EntityAquis;

// Object Struct
extern ObjectAquis *Aquis;

// Standard Entity Events
void Aquis_Update(void);
void Aquis_LateUpdate(void);
void Aquis_StaticUpdate(void);
void Aquis_Draw(void);
void Aquis_Create(void* data);
void Aquis_StageLoad(void);
void Aquis_EditorDraw(void);
void Aquis_EditorLoad(void);
void Aquis_Serialize(void);

// Extra Entity Functions
void Aquis_DebugSpawn(void);
void Aquis_DebugDraw(void);

void Aquis_CheckPlayerCollisions(void);
void Aquis_CheckOnScreen(void);

void Aquis_State_Setup(void);
void Aquis_Unknown5(void);
void Aquis_Unknown6(void);
void Aquis_Unknown7(void);
void Aquis_Unknown8(void);
void Aquis_Unknown9(void);

void Aquis_State_Shot(void);

#endif //!OBJ_AQUIS_H
