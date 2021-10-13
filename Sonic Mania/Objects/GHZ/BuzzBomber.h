#ifndef OBJ_BUZZBOMBER_H
#define OBJ_BUZZBOMBER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint16 spriteIndex;
} ObjectBuzzBomber;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 shotRange;
    int timer;
    uint8 field_64;
    Vector2 startPos;
    uint8 startDir;
    Entity *projectile;
    Animator data1;
    Animator animator2;
    Animator data3;
    Hitbox rangeHitbox;
} EntityBuzzBomber;

// Object Struct
extern ObjectBuzzBomber *BuzzBomber;

// Standard Entity Events
void BuzzBomber_Update(void);
void BuzzBomber_LateUpdate(void);
void BuzzBomber_StaticUpdate(void);
void BuzzBomber_Draw(void);
void BuzzBomber_Create(void* data);
void BuzzBomber_StageLoad(void);
void BuzzBomber_EditorDraw(void);
void BuzzBomber_EditorLoad(void);
void BuzzBomber_Serialize(void);

// Extra Entity Functions
void BuzzBomber_DebugDraw(void);
void BuzzBomber_DebugSpawn(void);

void BuzzBomber_CheckOnScreen(void);
void BuzzBomber_Unknown1(void);
void BuzzBomber_Unknown2(void);
void BuzzBomber_Unknown3(void);
void BuzzBomber_Unknown4(void);
void BuzzBomber_Unknown5(void);
void BuzzBomber_Unknown6(void);
void BuzzBomber_Unknown7(void);

#endif //!OBJ_BUZZBOMBER_H
