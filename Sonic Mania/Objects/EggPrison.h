#ifndef OBJ_EGGPRISON_H
#define OBJ_EGGPRISON_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
    ushort sfxDestroy;
    ushort sfxSpring;
} ObjectEggPrison;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)();
    int type;
    int timer;
    int angleY;
    int offsetY;
    int activated;
    int field_70;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    AnimationData data1;
    AnimationData data2;
    AnimationData data3;
    AnimationData data4;
} EntityEggPrison;

// Object Struct
extern ObjectEggPrison *EggPrison;

// Standard Entity Events
void EggPrison_Update();
void EggPrison_LateUpdate();
void EggPrison_StaticUpdate();
void EggPrison_Draw();
void EggPrison_Create(void* data);
void EggPrison_StageLoad();
void EggPrison_EditorDraw();
void EggPrison_EditorLoad();
void EggPrison_Serialize();

// Extra Entity Functions
void EggPrison_HandleTileCollisions();
void EggPrison_Activated();
void EggPrison_Unknown1();
void EggPrison_Unknown2();
void EggPrison_Unknown3();
void EggPrison_Unknown4();
void EggPrison_Unknown5();

#endif //!OBJ_EGGPRISON_H
