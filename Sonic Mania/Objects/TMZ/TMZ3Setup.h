#ifndef OBJ_TMZ3SETUP_H
#define OBJ_TMZ3SETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value;
    int dword8;
    int field_C;
    int aniTileFrame;
    ushort aniTiles;
} ObjectTMZ3Setup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTMZ3Setup;

// Object Struct
extern ObjectTMZ3Setup *TMZ3Setup;

// Standard Entity Events
void TMZ3Setup_Update(void);
void TMZ3Setup_LateUpdate(void);
void TMZ3Setup_StaticUpdate(void);
void TMZ3Setup_Draw(void);
void TMZ3Setup_Create(void* data);
void TMZ3Setup_StageLoad(void);
void TMZ3Setup_EditorDraw(void);
void TMZ3Setup_EditorLoad(void);
void TMZ3Setup_Serialize(void);

// Extra Entity Functions
void TMZ3Setup_DrawLayerCB_A(void);
void TMZ3Setup_DrawLayerCB_B(void);

#endif //!OBJ_TMZ3SETUP_H
