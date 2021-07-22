#ifndef OBJ_HANDLAUNCHER_H
#define OBJ_HANDLAUNCHER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    bool32 dunkeyMode;
    ushort sfxDunkey;
} ObjectHandLauncher;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int speed;
    byte activePlayers;
    Vector2 startPos;
    Vector2 playerPos;
    Vector2 field_74;
    int field_7C;
    int field_80;
    Animator animator1;
    Animator animator2;
} EntityHandLauncher;

// Object Struct
extern ObjectHandLauncher *HandLauncher;

// Standard Entity Events
void HandLauncher_Update(void);
void HandLauncher_LateUpdate(void);
void HandLauncher_StaticUpdate(void);
void HandLauncher_Draw(void);
void HandLauncher_Create(void* data);
void HandLauncher_StageLoad(void);
void HandLauncher_EditorDraw(void);
void HandLauncher_EditorLoad(void);
void HandLauncher_Serialize(void);

// Extra Entity Functions
void HandLauncher_CheckPlayerCollisions(void);
bool32 HandLauncher_Unknown1(void);

void HandLauncher_State_Release(void);
void HandLauncher_Unknown3(void);
void HandLauncher_Unknown5(void);
void HandLauncher_Unknown6(void);
void HandLauncher_Unknown7(void);
void HandLauncher_Unknown8(void);

#endif //!OBJ_HANDLAUNCHER_H
