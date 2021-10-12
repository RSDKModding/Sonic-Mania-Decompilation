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
    ushort aniFrames;
    ushort sfxExplosion;
    ushort sfxPeck;
} ObjectWoodrow;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    byte type;
    int distUp;
    int distDown;
    byte field_68;
    byte bombCount;
    byte field_6A;
    byte field_6B;
    int field_6C;
    Vector2 startPos;
    byte startDir;
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
void Woodrow_EditorDraw(void);
void Woodrow_EditorLoad(void);
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
