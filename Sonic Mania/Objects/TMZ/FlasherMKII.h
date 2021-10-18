#ifndef OBJ_FLASHERMKII_H
#define OBJ_FLASHERMKII_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxFlop;
    uint16 sfxZap;
} ObjectFlasherMKII;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 orientation;
    int32 type;
    int32 timer;
    Vector2 startPos;
    Animator animator;
} EntityFlasherMKII;

// Object Struct
extern ObjectFlasherMKII *FlasherMKII;

// Standard Entity Events
void FlasherMKII_Update(void);
void FlasherMKII_LateUpdate(void);
void FlasherMKII_StaticUpdate(void);
void FlasherMKII_Draw(void);
void FlasherMKII_Create(void* data);
void FlasherMKII_StageLoad(void);
void FlasherMKII_EditorDraw(void);
void FlasherMKII_EditorLoad(void);
void FlasherMKII_Serialize(void);

// Extra Entity Functions
void FlasherMKII_DebugSpawn(void);
void FlasherMKII_DebugDraw(void);

void FlasherMKII_CheckOnScreen(void);

void FlasherMKII_HandlePlayerCollisions(void);
void FlasherMKII_HandleHarmPlayerCollisions(void);

void FlasherMKII_State_Unknown1(void);
void FlasherMKII_State_Unknown2(void);
void FlasherMKII_State_Unknown3(void);
void FlasherMKII_State_Unknown4(void);
void FlasherMKII_State_Unknown5(void);

#endif //!OBJ_FLASHERMKII_H
