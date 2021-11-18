#ifndef OBJ_TURNTABLE_H
#define OBJ_TURNTABLE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectTurntable;

// Entity Class
typedef struct {
    RSDK_ENTITY
    uint8 type;
    int32 angleVel;
    uint8 activePlayers;
    Vector2 size;
    Vector2 origin;
    int32 unused;
    int32 playerAngles[PLAYER_MAX];
    int32 playerDistance[PLAYER_MAX];
    int32 playerFrames[PLAYER_MAX];
    Hitbox hitbox;
    Animator animator;
} EntityTurntable;

// Object Struct
extern ObjectTurntable *Turntable;

// Standard Entity Events
void Turntable_Update(void);
void Turntable_LateUpdate(void);
void Turntable_StaticUpdate(void);
void Turntable_Draw(void);
void Turntable_Create(void* data);
void Turntable_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Turntable_EditorDraw(void);
void Turntable_EditorLoad(void);
#endif
void Turntable_Serialize(void);

// Extra Entity Functions
void Turntable_SetupSize(void);


#endif //!OBJ_TURNTABLE_H
