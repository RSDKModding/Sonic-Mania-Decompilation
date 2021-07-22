#ifndef OBJ_EGGPRISON_H
#define OBJ_EGGPRISON_H

#include "SonicMania.h"

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
    StateMachine(state);
    int type;
    int timer;
    int angleY;
    int offsetY;
    int activated;
    int field_70;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Animator data1;
    Animator data2;
    Animator data3;
    Animator data4;
} EntityEggPrison;

// Object Struct
extern ObjectEggPrison *EggPrison;

// Standard Entity Events
void EggPrison_Update(void);
void EggPrison_LateUpdate(void);
void EggPrison_StaticUpdate(void);
void EggPrison_Draw(void);
void EggPrison_Create(void* data);
void EggPrison_StageLoad(void);
void EggPrison_EditorDraw(void);
void EggPrison_EditorLoad(void);
void EggPrison_Serialize(void);

// Extra Entity Functions
void EggPrison_HandleTileCollisions(void);
void EggPrison_Activated(void);
void EggPrison_Unknown1(void);
void EggPrison_Unknown2(void);
void EggPrison_Unknown3(void);
void EggPrison_Unknown4(void);
void EggPrison_Unknown5(void);

#endif //!OBJ_EGGPRISON_H
