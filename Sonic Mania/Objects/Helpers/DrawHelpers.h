#ifndef OBJ_DRAWHELPERS_H
#define OBJ_DRAWHELPERS_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
} ObjectDrawHelpers;

// Entity Class
typedef struct {
    RSDK_ENTITY
} EntityDrawHelpers;

// Object Entity
extern ObjectDrawHelpers *DrawHelpers;

// Standard Entity Events
void DrawHelpers_Update(void);
void DrawHelpers_LateUpdate(void);
void DrawHelpers_StaticUpdate(void);
void DrawHelpers_Draw(void);
void DrawHelpers_Create(void *data);
void DrawHelpers_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void DrawHelpers_EditorDraw(void);
void DrawHelpers_EditorLoad(void);
#endif
void DrawHelpers_Serialize(void);

// Extra Entity Functions
void DrawHelpers_DrawHitboxOutline(uint32 colour, uint8 dir, int32 x, int32 y, Hitbox *hitbox);
void DrawHelpers_DrawArrow(uint32 colour, int32 x1, int32 y1, int32 x2, int32 y2);
void DrawHelpers_DrawDebug4(uint32 colour, int32 a2, int32 alpha, int32 x1, int32 y1, int32 x2, int32 y2);
void DrawHelpers_DrawDebug5(uint32 colour, int32 x, int32 y);

//Custom Helpers I added for editor stuff
void DrawHelpers_DrawRectOutline(uint32 colour, int32 x, int32 y, int32 sizeX, int32 sizeY);
void DrawHelpers_DrawArenaBounds(uint32 colour, uint8 flags, int32 left, int32 top, int32 right, int32 bottom);

#endif //! OBJ_DRAWHELPERS_H
