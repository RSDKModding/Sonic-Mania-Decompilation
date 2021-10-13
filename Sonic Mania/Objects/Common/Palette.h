#ifndef OBJ_PALETTE_H
#define OBJ_PALETTE_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    uint8 field_4;
    uint8 field_5;
    uint8 count;
    uint8 field_7;
} ObjectPalette;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int state;
    TextInfo paletteFile;
    uint16 rowFlags;
    uint8 bankID;
    bool32 loadOnce;
} EntityPalette;

// Object Struct
extern ObjectPalette *Palette;

// Standard Entity Events
void Palette_Update(void);
void Palette_LateUpdate(void);
void Palette_StaticUpdate(void);
void Palette_Draw(void);
void Palette_Create(void *data);
void Palette_StageLoad(void);
void Palette_EditorDraw(void);
void Palette_EditorLoad(void);
void Palette_Serialize(void);
#endif

// Extra Entity Functions

#endif //! OBJ_PALETTE_H
