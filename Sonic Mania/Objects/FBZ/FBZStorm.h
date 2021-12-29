#ifndef OBJ_FBZSTORM_H
#define OBJ_FBZSTORM_H

#include "SonicMania.h"

// Object Class
struct ObjectFBZStorm {
    RSDK_OBJECT
    TABLE(int32 thunderFade[20], { 64, 128, 192, 256, 256, 128, 0, 32, 64, 128, 192, 256, 256, 192, 128, 96, 64, 32, 16, 0 });
    Vector2 positions[0x100];
    uint16 aniFrames;
    uint16 sfxRain;
    uint8 playingRainSFX;
    uint16 sfxThunder;
    STATIC(uint8 srcPal, 1);
};

// Entity Class
struct EntityFBZStorm {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 blendAmount;
    int32 stormAngle[4];
    int32 screenPosX[4];
    bool32 enabled;
    Animator animator;
};

// Object Struct
extern ObjectFBZStorm *FBZStorm;

// Standard Entity Events
void FBZStorm_Update(void);
void FBZStorm_LateUpdate(void);
void FBZStorm_StaticUpdate(void);
void FBZStorm_Draw(void);
void FBZStorm_Create(void* data);
void FBZStorm_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void FBZStorm_EditorDraw(void);
void FBZStorm_EditorLoad(void);
#endif
void FBZStorm_Serialize(void);

// Extra Entity Functions
void FBZStorm_State_WaitForActive(void);
void FBZStorm_State_StormStart(void);
void FBZStorm_State_StormFinish(void);
void FBZStorm_State_Storming(void);
void FBZStorm_State_Thunder(void);

#endif //!OBJ_FBZSTORM_H
