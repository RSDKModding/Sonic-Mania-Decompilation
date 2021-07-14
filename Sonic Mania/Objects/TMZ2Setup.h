#ifndef OBJ_TMZ2SETUP_H
#define OBJ_TMZ2SETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    bool32 paletteInit;
    int aniTileDelay;
    int aniTileFrame;
    int dword10;
    int dword14;
    ushort aniTiles;
    ushort dynTiles;
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
void TMZ2Setup_Create(void* data);
void TMZ2Setup_StageLoad(void);
void TMZ2Setup_EditorDraw(void);
void TMZ2Setup_EditorLoad(void);
void TMZ2Setup_Serialize(void);

// Extra Entity Functions
void TMZ2Setup_DrawLayerCB_A(void);
void TMZ2Setup_DrawLayerCB_B(void);

void TMZ2_DrawDynTiles1(void);
void TMZ2_DrawDynTiles2(void);

#endif //!OBJ_TMZ2SETUP_H
