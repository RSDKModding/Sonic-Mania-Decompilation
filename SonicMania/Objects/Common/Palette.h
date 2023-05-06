#ifndef OBJ_PALETTE_H
#define OBJ_PALETTE_H

#include "Game.h"

#if MANIA_USE_PLUS

typedef enum {
    PALETTE_BANK_0,
    PALETTE_BANK_1,
    PALETTE_BANK_2,
    PALETTE_BANK_3,
    PALETTE_BANK_4,
    PALETTE_BANK_5,
    PALETTE_BANK_6,
    PALETTE_BANK_7,
} PaletteBankIDs;

// Object Class
struct ObjectPalette {
    RSDK_OBJECT
    uint16 aniFrames;
    uint8 count;
};

// Entity Class
struct EntityPalette {
    RSDK_ENTITY
    int32 state;
    String paletteFile;
    uint16 rowFlags;
    uint8 bankID;
    bool32 loadOnce;
    Animator animator;
};

// Object Struct
extern ObjectPalette *Palette;

// Standard Entity Events
void Palette_Update(void);
void Palette_LateUpdate(void);
void Palette_StaticUpdate(void);
void Palette_Draw(void);
void Palette_Create(void *data);
void Palette_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Palette_EditorDraw(void);
void Palette_EditorLoad(void);
#endif
void Palette_Serialize(void);
#endif

// Extra Entity Functions

#endif //! OBJ_PALETTE_H
