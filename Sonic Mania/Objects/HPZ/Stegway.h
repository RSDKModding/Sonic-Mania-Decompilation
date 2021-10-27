#ifndef OBJ_STEGWAY_H
#define OBJ_STEGWAY_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitbox2;
    uint16 aniFrames;
    uint16 sfxRev;
    uint16 sfxRelease;
} ObjectStegway;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    uint8 startDir;
    int32 timer;
    int32 field_6C;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    bool32 dustFlag;
    int32 field_BC;
    bool32 flag;
} EntityStegway;

// Object Entity
extern ObjectStegway *Stegway;

// Standard Entity Events
void Stegway_Update(void);
void Stegway_LateUpdate(void);
void Stegway_StaticUpdate(void);
void Stegway_Draw(void);
void Stegway_Create(void* data);
void Stegway_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Stegway_EditorDraw(void);
void Stegway_EditorLoad(void);
#endif
void Stegway_Serialize(void);

// Extra Entity Functions
void Stegway_DebugSpawn(void);
void Stegway_DebugDraw(void);

void Stegway_CheckOnScreen(void);
void Stegway_HandlePlayerInteractions(void);
void Stegway_SetupAnims(char type, bool32 force);

void Stegway_State_Setup(void);
void Stegway_Unknown6(void);
void Stegway_Unknown7(void);
void Stegway_Unknown8(void);
void Stegway_Unknown9(void);
void Stegway_Unknown10(void);

#endif //!OBJ_STEGWAY_H
