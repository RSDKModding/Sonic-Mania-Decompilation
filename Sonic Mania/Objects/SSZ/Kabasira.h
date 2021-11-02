#ifndef OBJ_KABASIRA_H
#define OBJ_KABASIRA_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    Vector2 checkRange;
    uint16 aniFrames;
    uint16 sfxPon;
    uint16 sfxExplosion2;
} ObjectKabasira;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 field_5C;
    int32 field_60;
    Vector2 startPos;
    uint8 startDir;
    int32 timer;
    Animator animator1;
    Animator animator2;
    int32 field_A4;
} EntityKabasira;

// Object Struct
extern ObjectKabasira *Kabasira;

// Standard Entity Events
void Kabasira_Update(void);
void Kabasira_LateUpdate(void);
void Kabasira_StaticUpdate(void);
void Kabasira_Draw(void);
void Kabasira_Create(void* data);
void Kabasira_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Kabasira_EditorDraw(void);
void Kabasira_EditorLoad(void);
#endif
void Kabasira_Serialize(void);

// Extra Entity Functions
void Kabasira_DebugSpawn(void);
void Kabasira_DebugDraw(void);

bool32 Kabasira_HandleAnimations(uint8 angle);
void Kabasira_DrawSegment(int32 angle, int32 alpha);
void Kabasira_CheckPlayerCollisions(void);

void Kabasira_State_Setup(void);
void Kabasira_State_Unknown1(void);
void Kabasira_State2_Unknown(void);

#endif //!OBJ_KABASIRA_H
