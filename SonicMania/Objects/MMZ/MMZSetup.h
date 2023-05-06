#ifndef OBJ_MMZSETUP_H
#define OBJ_MMZSETUP_H

#include "Game.h"

// Object Class
struct ObjectMMZSetup {
    RSDK_OBJECT
    int32 pistonAniFrame;
    STATIC(int32 pistonAniDuration, 60);
    TABLE(int32 pistonAniDurationTable[12], { 9, 4, 4, 2, 2, 2, 9, 4, 4, 2, 2, 2 });
    uint8 paletteTimer;
    uint8 panelAniTimer;
    uint8 panelAniFrame;
    uint8 panelAniDuration;
    int32 lightsAniDuration;
    int32 spinLightsAniFrame;
    int32 generatorAniFrame;
    uint16 aniTiles;
};

// Entity Class
struct EntityMMZSetup {
    RSDK_ENTITY
};

// Object Struct
extern ObjectMMZSetup *MMZSetup;

// Standard Entity Events
void MMZSetup_Update(void);
void MMZSetup_LateUpdate(void);
void MMZSetup_StaticUpdate(void);
void MMZSetup_Draw(void);
void MMZSetup_Create(void *data);
void MMZSetup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MMZSetup_EditorDraw(void);
void MMZSetup_EditorLoad(void);
#endif
void MMZSetup_Serialize(void);

// Extra Entity Functions
void MMZSetup_StageFinish_EndAct1(void);
#if MANIA_USE_PLUS
void MMZSetup_StageFinish_EndAct2(void);
#endif

#endif //! OBJ_MMZSETUP_H
