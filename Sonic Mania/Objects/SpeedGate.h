#ifndef OBJ_SPEEDGATE_H
#define OBJ_SPEEDGATE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    int field_C;
    ushort spriteIndex;
    ushort sfx_StarPost;
} ObjectSpeedGate;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void(*state)();
    int id;
    int timer;
    int prize;
    int timer2;
    int timer3;
    int field_70;
    int field_74;
    Vector2 posUnknown;
    int field_80;
    int field_84;
    EntityPlayer *playerPtr;
    AnimationData baseData;
    AnimationData topData;
    AnimationData timerData;
    AnimationData finsData;
    AnimationData bubbleData;
    int activePlayers;
} EntitySpeedGate;

// Object Struct
extern ObjectSpeedGate *SpeedGate;

// Standard Entity Events
void SpeedGate_Update();
void SpeedGate_LateUpdate();
void SpeedGate_StaticUpdate();
void SpeedGate_Draw();
void SpeedGate_Create(void* data);
void SpeedGate_StageLoad();
void SpeedGate_EditorDraw();
void SpeedGate_EditorLoad();
void SpeedGate_Serialize();

// Extra Entity Functions
void SpeedGate_State_Unknown1();
void SpeedGate_State_Unknown2();
void SpeedGate_State_Unknown3();

#endif //!OBJ_SPEEDGATE_H
