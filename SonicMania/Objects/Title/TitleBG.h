#ifndef OBJ_TITLEBG_H
#define OBJ_TITLEBG_H

#include "Game.h"

typedef enum {
    TITLEBG_MOUNTAIN1,
    TITLEBG_MOUNTAIN2,
    TITLEBG_REFLECTION,
    TITLEBG_WATERSPARKLE,
    TITLEBG_WINGSHINE,
} TitleBGTypes;

// Object Class
struct ObjectTitleBG {
    RSDK_OBJECT
    int32 palTimer;
    int32 timer;
    int32 angle;
    uint16 aniFrames;
};

// Entity Class
struct EntityTitleBG {
    RSDK_ENTITY
    TitleBGTypes type;
    int32 timer;
    Animator animator;
};

// Object Struct
extern ObjectTitleBG *TitleBG;

// Standard Entity Events
void TitleBG_Update(void);
void TitleBG_LateUpdate(void);
void TitleBG_StaticUpdate(void);
void TitleBG_Draw(void);
void TitleBG_Create(void *data);
void TitleBG_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TitleBG_EditorDraw(void);
void TitleBG_EditorLoad(void);
#endif
void TitleBG_Serialize(void);

// Extra Entity Functions
void TitleBG_SetupFX(void);
void TitleBG_Scanline_Clouds(ScanlineInfo *scanlines);
void TitleBG_Scanline_Island(ScanlineInfo *scanlines);

#endif //! OBJ_TITLEBG_H
