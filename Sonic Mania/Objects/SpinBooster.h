#ifndef OBJ_SPINBOOSTER_H
#define OBJ_SPINBOOSTER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
} ObjectSpinBooster;

// Entity Class
typedef struct {
    RSDK_ENTITY
    byte autoGrip;
    byte bias;
    int size;
    int boostPower;
    bool32 boostAlways;
    bool32 forwardOnly;
    bool32 playSound;
    bool32 allowTubeInput;
    byte activePlayers;
    int field_78;
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
int SpinBooster_GetRollDir(EntityPlayer *player);
void SpinBooster_HandleRollDir(EntityPlayer *player);
void SpinBooster_ApplyRollVelocity(EntityPlayer *player);
void SpinBooster_Unknown4(uint colour, int X1, int Y1, int X2, int Y2);
void SpinBooster_DrawSprites(void);
void SpinBooster_HandleForceRoll(EntityPlayer *player);


#endif //!OBJ_SPINBOOSTER_H
