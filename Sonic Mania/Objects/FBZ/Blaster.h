#ifndef OBJ_BLASTER_H
#define OBJ_BLASTER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxRange;
    Hitbox hitboxProjectile;
    uint16 aniFrames;
} ObjectBlaster;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 field_5C;
    uint8 field_5D;
    uint16 timer;
    uint16 timer2;
    uint16 field_62;
    Vector2 startPos;
    uint8 startDir;
    Animator animator;
} EntityBlaster;

// Object Struct
extern ObjectBlaster *Blaster;

// Standard Entity Events
void Blaster_Update(void);
void Blaster_LateUpdate(void);
void Blaster_StaticUpdate(void);
void Blaster_Draw(void);
void Blaster_Create(void* data);
void Blaster_StageLoad(void);
void Blaster_EditorDraw(void);
void Blaster_EditorLoad(void);
void Blaster_Serialize(void);

// Extra Entity Functions
void Blaster_DebugSpawn(void);
void Blaster_DebugDraw(void);

void Blaster_HandlePlayerInteractions(void);
void Blaster_CheckOnScreen(void);

void Blaster_State_Setup(void);
void Blaster_State_Unknown1(void);
void Blaster_State_Unknown2(void);
void Blaster_State_Unknown3(void);
void Blaster_State_MagnetAttract(void);
void Blaster_State_Unknown4(void);
void Blaster_State_Unknown5(void);
void Blaster_State_Unknown6(void);
void Blaster_State_Unknown7(void);
void Blaster_State_Unknown8(void);

#endif //!OBJ_BLASTER_H
