#ifndef OBJ_MMZSETUP_H
#define OBJ_MMZSETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int aniTileFrameB;
    STATIC(int aniTileDelay3, 60);
    TABLE(int aniTileDelays[12], { 9, 4, 4, 2, 2, 2, 9, 4, 4, 2, 2, 2 });
    uint8 paletteTimer;
    uint8 aniTileDelay;
    uint8 aniTileFrameA;
    uint8 aniTileDelay2;
    int aniTileDelay4;
    int aniTileFrameC;
    int aniTileFrameD;
    uint16 aniTiles;
} ObjectMMZSetup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMMZSetup;

// Object Struct
extern ObjectMMZSetup *MMZSetup;

// Standard Entity Events
void MMZSetup_Update(void);
void MMZSetup_LateUpdate(void);
void MMZSetup_StaticUpdate(void);
void MMZSetup_Draw(void);
void MMZSetup_Create(void* data);
void MMZSetup_StageLoad(void);
void MMZSetup_EditorDraw(void);
void MMZSetup_EditorLoad(void);
void MMZSetup_Serialize(void);

// Extra Entity Functions
void MMZSetup_StageFinishCB_Act1(void);
#if RETRO_USE_PLUS
void MMZSetup_StageFinishCB_Act2(void);
#endif

#endif //!OBJ_MMZSETUP_H
