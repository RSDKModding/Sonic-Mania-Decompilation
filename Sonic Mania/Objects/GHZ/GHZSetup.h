#ifndef OBJ_GHZSETUP_H
#define OBJ_GHZSETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int waterFadeAmount;
    int delayA; //= 30
    int frameA;
    int delayListA[8];
    int delayB; //= 60
    int frameB;
    int delayListB[16];
    ushort aniTiles;
} ObjectGHZSetup;

// Entity Class
typedef struct {
    RSDK_ENTITY
} EntityGHZSetup;

// Object Struct
extern ObjectGHZSetup *GHZSetup;

// Standard Entity Events
void GHZSetup_Update(void);
void GHZSetup_LateUpdate(void);
void GHZSetup_StaticUpdate(void);
void GHZSetup_Draw(void);
void GHZSetup_Create(void* data);
void GHZSetup_StageLoad(void);
void GHZSetup_EditorDraw(void);
void GHZSetup_EditorLoad(void);
void GHZSetup_Serialize(void);

// Extra Entity Functions
void GHZSetup_SetupStartingBG(void);
void GHZSetup_SetupActTransition(void);
void GHZSetup_HandleActTransition(void);
void GHZSetup_SetupBGSwitchA1(void);
void GHZSetup_SetupBGSwitchA2(void);
void GHZSetup_SetupBGSwitchB1(void);
void GHZSetup_SetupBGSwitchB2(void);
void GHZSetup_SpawnGHZ2Outro(void);

#endif //!OBJ_GHZSETUP_H
