#ifndef OBJ_BSS_PALETTE_H
#define OBJ_BSS_PALETTE_H

#include "Game.h"

// Object Class
struct ObjectBSS_Palette {
    RSDK_OBJECT
    int32 startColorID;
    int32 skyAlpha;
    int32 globeAlpha;
    uint16 aniFrames;
};

// Entity Class
struct EntityBSS_Palette {
    RSDK_ENTITY
    bool32 useStageConfig;
    int32 paletteID;
    uint8 skyAlpha;
    uint8 globeAlpha;
    color playfieldA;
    color playfieldB;
    color bgColor1;
    color bgColor2;
    color bgColor3;
    Animator animator;
};

// Object Struct
extern ObjectBSS_Palette *BSS_Palette;

// Standard Entity Events
void BSS_Palette_Update(void);
void BSS_Palette_LateUpdate(void);
void BSS_Palette_StaticUpdate(void);
void BSS_Palette_Draw(void);
void BSS_Palette_Create(void *data);
void BSS_Palette_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void BSS_Palette_EditorDraw(void);
void BSS_Palette_EditorLoad(void);
#endif
void BSS_Palette_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_BSS_PALETTE_H
