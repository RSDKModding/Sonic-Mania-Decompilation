#ifndef OBJ_FBZSTORM_H
#define OBJ_FBZSTORM_H

#include "Game.h"

// Object Class
struct ObjectFBZStorm {
    RSDK_OBJECT
    TABLE(int32 thunderFade[20],
          { 0x40, 0x80, 0xC0, 0x100, 0x100, 0x80, 0x00, 0x20, 0x40, 0x80, 0xC0, 0x100, 0x100, 0xC0, 0x80, 0x60, 0x40, 0x20, 0x10, 0x00 });
    Vector2 raindropPositions[0x100];
    uint16 aniFrames;
    uint16 sfxRain;
    bool32 playingRainSfx; // should be uint8 according to the static vars file, bool32 actually *breaks* outsideBank, but that's how mania is so..
    uint16 sfxThunder;
    STATIC(uint8 outsideBank, 1);
};

// Entity Class
struct EntityFBZStorm {
    RSDK_ENTITY
    StateMachine(state);
    int32 thunderTimer;
    int32 blendAmount;
    int32 stormAngle[SCREEN_COUNT];
    int32 screenPosX[SCREEN_COUNT];
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
void FBZStorm_Create(void *data);
void FBZStorm_StageLoad(void);
#if GAME_INCLUDE_EDITOR
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

#endif //! OBJ_FBZSTORM_H
