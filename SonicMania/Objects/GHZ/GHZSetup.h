#ifndef OBJ_GHZSETUP_H
#define OBJ_GHZSETUP_H

#include "Game.h"

typedef enum {
    GHZ_BG_OUTSIDE,
    GHZ_BG_CAVES,
} BGSwitchIDsGHZ;

typedef enum { GHZ_DECORATION_BRIDGEPOST } DecorationTypesGHZ;

// Object Class
struct ObjectGHZSetup {
    RSDK_OBJECT
    int32 paletteTimer;
    STATIC(int32 sunFlowerTimer, 30);
    int32 sunFlowerFrame;
    TABLE(int32 sunFlowerDurationTable[8], { 30, 2, 2, 2, 30, 2, 2, 2 });
    STATIC(int32 extendFlowerTimer, 60);
    int32 extendFlowerFrame;
    TABLE(int32 extendFlowerDurationTable[16], { 120, 2, 2, 2, 8, 2, 2, 2, 120, 2, 2, 2, 8, 2, 2, 2 });
    uint16 aniTiles;
};

// Entity Class
struct EntityGHZSetup {
    RSDK_ENTITY
};

// Object Struct
extern ObjectGHZSetup *GHZSetup;

// Standard Entity Events
void GHZSetup_Update(void);
void GHZSetup_LateUpdate(void);
void GHZSetup_StaticUpdate(void);
void GHZSetup_Draw(void);
void GHZSetup_Create(void *data);
void GHZSetup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void GHZSetup_EditorDraw(void);
void GHZSetup_EditorLoad(void);
#endif
void GHZSetup_Serialize(void);

// Extra Entity Functions
void GHZSetup_SetupAct1BG(void);
void GHZSetup_StageFinish_EndAct1(void);
void GHZSetup_HandleActTransition(void);
void GHZSetup_BGSwitch_Outside_Act2(void);
void GHZSetup_BGSwitch_Caves_Act2(void);
void GHZSetup_BGSwitch_Outside_Act1(void);
void GHZSetup_BGSwitch_Caves_Act1(void);
void GHZSetup_StageFinish_EndAct2(void);

#endif //! OBJ_GHZSETUP_H
