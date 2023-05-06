#ifndef OBJ_TMZ3SETUP_H
#define OBJ_TMZ3SETUP_H

#include "Game.h"

// Object Class
struct ObjectTMZ3Setup {
    RSDK_OBJECT
    int32 palDuration;
    int32 palTimer;
    int32 unused;
    int32 bannerAniFrame;
    uint16 aniTiles;
};

// Entity Class
struct EntityTMZ3Setup {
    RSDK_ENTITY
};

// Object Struct
extern ObjectTMZ3Setup *TMZ3Setup;

// Standard Entity Events
void TMZ3Setup_Update(void);
void TMZ3Setup_LateUpdate(void);
void TMZ3Setup_StaticUpdate(void);
void TMZ3Setup_Draw(void);
void TMZ3Setup_Create(void *data);
void TMZ3Setup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TMZ3Setup_EditorDraw(void);
void TMZ3Setup_EditorLoad(void);
#endif
void TMZ3Setup_Serialize(void);

// Extra Entity Functions
void TMZ3Setup_DrawHook_ApplyDynTilesPalette(void);
void TMZ3Setup_DrawHook_RemoveDynTilesPalette(void);

#endif //! OBJ_TMZ3SETUP_H
