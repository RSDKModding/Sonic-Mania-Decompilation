#ifndef OBJ_BUZZBOMBER_H
#define OBJ_BUZZBOMBER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    ushort spriteIndex;
} ObjectBuzzBomber;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void(*state)();
    byte shotRange;
    int timer;
    byte field_64;
    Vector2 startPos;
    byte startDir;
    Entity *projectile;
    AnimationData data1;
    AnimationData data2;
    AnimationData data3;
    Hitbox rangeHitbox;
} EntityBuzzBomber;

// Object Struct
extern ObjectBuzzBomber *BuzzBomber;

// Standard Entity Events
void BuzzBomber_Update();
void BuzzBomber_LateUpdate();
void BuzzBomber_StaticUpdate();
void BuzzBomber_Draw();
void BuzzBomber_Create(void* data);
void BuzzBomber_StageLoad();
void BuzzBomber_EditorDraw();
void BuzzBomber_EditorLoad();
void BuzzBomber_Serialize();

// Extra Entity Functions
void BuzzBomber_DebugDraw();
void BuzzBomber_DebugSpawn();

void BuzzBomber_CheckOnScreen();
void BuzzBomber_Unknown1();
void BuzzBomber_Unknown2();
void BuzzBomber_Unknown3();
void BuzzBomber_Unknown4();
void BuzzBomber_Unknown5();
void BuzzBomber_Unknown6();
void BuzzBomber_Unknown7();

#endif //!OBJ_BUZZBOMBER_H
