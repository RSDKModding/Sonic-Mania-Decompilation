#ifndef OBJ_SPINBOOSTER_H
#define OBJ_SPINBOOSTER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectSpinBooster;

// Entity Class
typedef struct {
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
    int32 field_78;
    Animator animator;
} EntitySpinBooster;

// Object Struct
extern ObjectSpinBooster *SpinBooster;

// Standard Entity Events
void SpinBooster_Update(void);
void SpinBooster_LateUpdate(void);
void SpinBooster_StaticUpdate(void);
void SpinBooster_Draw(void);
void SpinBooster_Create(void* data);
void SpinBooster_StageLoad(void);
void SpinBooster_EditorDraw(void);
void SpinBooster_EditorLoad(void);
void SpinBooster_Serialize(void);

// Extra Entity Functions
int32 SpinBooster_GetRollDir(EntityPlayer *player);
void SpinBooster_HandleRollDir(EntityPlayer *player);
void SpinBooster_ApplyRollVelocity(EntityPlayer *player);
void SpinBooster_Unknown4(uint32 colour, int32 X1, int32 Y1, int32 X2, int32 Y2);
void SpinBooster_DrawSprites(void);
void SpinBooster_HandleForceRoll(EntityPlayer *player);


#endif //!OBJ_SPINBOOSTER_H
