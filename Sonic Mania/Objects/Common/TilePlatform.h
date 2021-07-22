#ifndef OBJ_TILEPLATFORM_H
#define OBJ_TILEPLATFORM_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
} ObjectTilePlatform;

// Entity Class
typedef struct {
    MANIA_PLATFORM_BASE
    Vector2 size;
    ushort tiles[0x100];
} EntityTilePlatform;

// Object Struct
extern ObjectTilePlatform *TilePlatform;

// Standard Entity Events
void TilePlatform_Update(void);
void TilePlatform_LateUpdate(void);
void TilePlatform_StaticUpdate(void);
void TilePlatform_Draw(void);
void TilePlatform_Create(void* data);
void TilePlatform_StageLoad(void);
void TilePlatform_EditorDraw(void);
void TilePlatform_EditorLoad(void);
void TilePlatform_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TILEPLATFORM_H
