#ifndef OBJ_TMZ3SETUP_H
#define OBJ_TMZ3SETUP_H

#include "SonicMania.h"

// Object Class
struct ObjectTMZ3Setup {
    RSDK_OBJECT
    int32 value;
    int32 dword8;
    int32 field_C;
    int32 aniTileFrame;
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
void TMZ3Setup_Create(void* data);
void TMZ3Setup_StageLoad(void);
void TMZ3Setup_EditorDraw(void);
void TMZ3Setup_EditorLoad(void);
void TMZ3Setup_Serialize(void);

// Extra Entity Functions
void TMZ3Setup_DrawLayerCB_A(void);
void TMZ3Setup_DrawLayerCB_B(void);

#endif //!OBJ_TMZ3SETUP_H
