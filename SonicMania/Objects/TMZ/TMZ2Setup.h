#ifndef OBJ_TMZ2SETUP_H
#define OBJ_TMZ2SETUP_H

#include "Game.h"

typedef enum { TMZ2_WARPDOOR_EFFECT_MIST = 1 } TMZ2WarpDoorEffects;

// Object Class
typedef struct {
    RSDK_OBJECT
    bool32 paletteInit;
    int32 palDuration;
    int32 palTimer;
    int32 unused1;
    int32 unused2;
    uint16 aniTiles;
    uint16 dynTiles;
} ObjectTMZ2Setup;

// Entity Class
typedef struct {
    RSDK_ENTITY
} EntityTMZ2Setup;

// Object Struct
extern ObjectTMZ2Setup *TMZ2Setup;

// Standard Entity Events
void TMZ2Setup_Update(void);
void TMZ2Setup_LateUpdate(void);
void TMZ2Setup_StaticUpdate(void);
void TMZ2Setup_Draw(void);
void TMZ2Setup_Create(void *data);
void TMZ2Setup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TMZ2Setup_EditorDraw(void);
void TMZ2Setup_EditorLoad(void);
#endif
void TMZ2Setup_Serialize(void);

// Extra Entity Functions
void TMZ2Setup_DrawHook_ApplyDynTilesPalette(void);
void TMZ2Setup_DrawHook_RemoveDynTilesPalette(void);

void TMZ2Setup_DrawDynTiles_Eggman(void);
void TMZ2Setup_DrawDynTiles_Ruby(void);

#endif //! OBJ_TMZ2SETUP_H
