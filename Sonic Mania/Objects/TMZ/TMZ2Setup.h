#ifndef OBJ_TMZ2SETUP_H
#define OBJ_TMZ2SETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    bool32 paletteInit;
    int32 palDelay;
    int32 palTimer;
    int32 dword10;
    int32 dword14;
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
void TMZ2Setup_Create(void* data);
void TMZ2Setup_StageLoad(void);
void TMZ2Setup_EditorDraw(void);
void TMZ2Setup_EditorLoad(void);
void TMZ2Setup_Serialize(void);

// Extra Entity Functions
void TMZ2Setup_DrawLayerCB_A(void);
void TMZ2Setup_DrawLayerCB_B(void);

void TMZ2_DrawDynTiles_Eggman(void);
void TMZ2_DrawDynTiles_Ruby(void);

#endif //!OBJ_TMZ2SETUP_H
