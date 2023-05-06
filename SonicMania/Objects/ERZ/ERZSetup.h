#ifndef OBJ_ERZSETUP_H
#define OBJ_ERZSETUP_H

#include "Game.h"

// Object Class
struct ObjectERZSetup {
    RSDK_OBJECT
    int32 aniTileTimer;
    int32 timer;
    uint16 aniTiles;
};

// Entity Class
struct EntityERZSetup {
    RSDK_ENTITY
};

// Object Struct
extern ObjectERZSetup *ERZSetup;

// Standard Entity Events
void ERZSetup_Update(void);
void ERZSetup_LateUpdate(void);
void ERZSetup_StaticUpdate(void);
void ERZSetup_Draw(void);
void ERZSetup_Create(void *data);
void ERZSetup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ERZSetup_EditorDraw(void);
void ERZSetup_EditorLoad(void);
#endif
void ERZSetup_Serialize(void);

// Extra Entity Functions
void ERZSetup_Scanline_Sky(ScanlineInfo *scanlines);

#endif //! OBJ_ERZSETUP_H
