#ifndef OBJ_WOODROW_H
#define OBJ_WOODROW_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox4;
    uint16 aniFrames;
    uint16 sfxExplosion;
    uint16 sfxPeck;
} ObjectWoodrow;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    int32 distUp;
    int32 distDown;
    uint8 field_68;
    uint8 bombCount;
    uint8 field_6A;
    uint8 field_6B;
    int32 field_6C;
    Vector2 startPos;
    uint8 startDir;
    Entity *parent;
    Animator animator;
} EntityWoodrow;

// Object Struct
extern ObjectWoodrow *Woodrow;

// Standard Entity Events
void Woodrow_Update(void);
void Woodrow_LateUpdate(void);
void Woodrow_StaticUpdate(void);
void Woodrow_Draw(void);
void Woodrow_Create(void* data);
void Woodrow_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Woodrow_EditorDraw(void);
void Woodrow_EditorLoad(void);
#endif
void Woodrow_Serialize(void);

// Extra Entity Functions
void Woodrow_DebugSpawn(void);
void Woodrow_DebugDraw(void);

void Woodrow_CheckPlayerCollisions(void);
void Woodrow_CheckOnScreen(void);

void Woodrow_State_Setup(void);
void Woodrow_Unknown4(void);
void Woodrow_Unknown5(void);
void Woodrow_Unknown6(void);
void Woodrow_Unknown7(void);
void Woodrow_Unknown8(void);

#endif //!OBJ_WOODROW_H
