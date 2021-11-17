#ifndef OBJ_SPEEDGATE_H
#define OBJ_SPEEDGATE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    int32 field_C;
    uint16 aniFrames;
    uint16 sfxStarPost;
} ObjectSpeedGate;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 id;
    int32 timer;
    int32 prize;
    int32 timer2;
    int32 timer3;
    int32 field_70;
    int32 field_74;
    Vector2 posUnknown;
    int32 field_80;
    int32 field_84;
    EntityPlayer *playerPtr;
    Animator baseData;
    Animator topData;
    Animator timerData;
    Animator finsData;
    Animator bubbleData;
    int32 activePlayers;
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
#if RETRO_INCLUDE_EDITOR
void SpeedGate_EditorDraw(void);
void SpeedGate_EditorLoad(void);
#endif
void SpeedGate_Serialize(void);

// Extra Entity Functions
void SpeedGate_State_Unknown1(void);
void SpeedGate_State_Unknown2(void);
void SpeedGate_State_Unknown3(void);

#endif //!OBJ_SPEEDGATE_H
