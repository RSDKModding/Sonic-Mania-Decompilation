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
    void(*state)(void);
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
void SpeedGate_Update(void);
void SpeedGate_LateUpdate(void);
void SpeedGate_StaticUpdate(void);
void SpeedGate_Draw(void);
void SpeedGate_Create(void* data);
void SpeedGate_StageLoad(void);
void SpeedGate_EditorDraw(void);
void SpeedGate_EditorLoad(void);
void SpeedGate_Serialize(void);

// Extra Entity Functions
void SpeedGate_State_Unknown1(void);
void SpeedGate_State_Unknown2(void);
void SpeedGate_State_Unknown3(void);

#endif //!OBJ_SPEEDGATE_H
