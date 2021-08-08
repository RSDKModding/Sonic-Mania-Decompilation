#ifndef OBJ_OOZSETUP_H
#define OBJ_OOZSETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int palTimer;
    ushort aniTiles;
    TileLayer* bgPtr;
    int deformData[64]; //= { 1, 2, 1, 3, 1, 2, 2, 1, 2, 3, 1, 2, 1, 2, 0, 0, 2, 0, 3, 2, 2, 3, 2, 2, 1, 3, 0, 0, 1, 0, 1, 3, 1, 2, 1, 3, 1, 2, 2, 1,
                        //2, 3, 1, 2, 1, 2, 0, 0, 2, 0, 3, 2, 2, 3, 2, 2, 1, 3, 0, 0, 1, 0, 1, 3 };
    int aniTilesTimer;
    int aniTilesFrame;
    int aniTilesDelay; //= 60;
    int aniTileDelays[9]; //= { 60, 60, 3, 3, 3, 3, 3, 3, 4 };
    int value9;
    int fadeTimer;
    int flags;
    byte flameTimers[0x20000];
    byte *flameTimerPtrs[400];
    Vector2 flamePositions[400];
    ushort flameCount;
    byte activePlayers;
#if RETRO_USE_PLUS
    Animator flameAnimator;
    Animator animator;
    ushort solFrames;
    ushort splashFrames;
#else
    Animator flameAnimator;
    ushort solFrames;
#endif
    bool32 hasAchievement;
    Entity *cutscenePtr;
} ObjectOOZSetup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    byte type;
} EntityOOZSetup;

// Object Struct
extern ObjectOOZSetup *OOZSetup;

// Standard Entity Events
void OOZSetup_Update(void);
void OOZSetup_LateUpdate(void);
void OOZSetup_StaticUpdate(void);
void OOZSetup_Draw(void);
void OOZSetup_Create(void* data);
void OOZSetup_StageLoad(void);
void OOZSetup_EditorDraw(void);
void OOZSetup_EditorLoad(void);
void OOZSetup_Serialize(void);

// Extra Entity Functions
bool32 OOZSetup_CheckCB_Flame(void);
bool32 OOZSetup_CheckCB_Slide(void);
bool32 OOZSetup_CheckCB_Swim(void);

void OOZSetup_Unknown4(void);
void OOZSetup_Unknown5(void);
bool32 OOZSetup_Unknown6(int posY, int posX, int angle);

void OOZSetup_GenericTriggerCB(void);

void OOZSetup_PlayerState_OilPool(void);
void OOZSetup_PlayerState_OilStrip(void);
void OOZSetup_PlayerState_OilSlide(void);
void OOZSetup_PlayerState_OilFall(void);

#endif //!OBJ_OOZSETUP_H
