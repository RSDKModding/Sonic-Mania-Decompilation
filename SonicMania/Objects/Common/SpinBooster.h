#ifndef OBJ_SPINBOOSTER_H
#define OBJ_SPINBOOSTER_H

#include "Game.h"

// Object Class
struct ObjectSpinBooster {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntitySpinBooster {
    RSDK_ENTITY
    uint8 autoGrip;
    uint8 bias;
    int32 size;
    int32 boostPower;
    bool32 boostAlways;
    bool32 forwardOnly;
    bool32 playSound;
    bool32 allowTubeInput;
    uint8 activePlayers;
    int32 unused;
    Animator animator;
};

// Object Struct
extern ObjectSpinBooster *SpinBooster;

// Standard Entity Events
void SpinBooster_Update(void);
void SpinBooster_LateUpdate(void);
void SpinBooster_StaticUpdate(void);
void SpinBooster_Draw(void);
void SpinBooster_Create(void *data);
void SpinBooster_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SpinBooster_EditorDraw(void);
void SpinBooster_EditorLoad(void);
#endif
void SpinBooster_Serialize(void);

// Extra Entity Functions
int32 SpinBooster_GetRollDir(EntityPlayer *player);
void SpinBooster_HandleRollDir(EntityPlayer *player);
void SpinBooster_ApplyRollVelocity(EntityPlayer *player);
void SpinBooster_DrawArrow(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color);
void SpinBooster_DrawSprites(void);
void SpinBooster_HandleForceRoll(EntityPlayer *player);

#endif //! OBJ_SPINBOOSTER_H
