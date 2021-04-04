#ifndef OBJ_BSS_PALETTE_H
#define OBJ_BSS_PALETTE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int startColourID;
    int alpha1;
    int alpha2;
    int field_10;
} ObjectBSS_Palette;

// Entity Class
typedef struct {
    RSDK_ENTITY
    bool32 useStageConfig;
    int paletteID;
    byte skyAlpha;
    byte globeAlpha;
    colour playfieldA;
    colour playfieldB;
    colour bgColor1;
    colour bgColor2;
    colour bgColor3;
} EntityBSS_Palette;

// Object Struct
extern ObjectBSS_Palette *BSS_Palette;

// Standard Entity Events
void BSS_Palette_Update(void);
void BSS_Palette_LateUpdate(void);
void BSS_Palette_StaticUpdate(void);
void BSS_Palette_Draw(void);
void BSS_Palette_Create(void* data);
void BSS_Palette_StageLoad(void);
void BSS_Palette_EditorDraw(void);
void BSS_Palette_EditorLoad(void);
void BSS_Palette_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BSS_PALETTE_H
