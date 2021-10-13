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
void DrawHelpers_EditorDraw(void);
void DrawHelpers_EditorLoad(void);
void DrawHelpers_Serialize(void);

// Extra Entity Functions
void DrawHelpers_DrawHitboxOutline(uint32 colour, uint8 dir, int32 x, int32 y, Hitbox *hitbox);
void DrawHelpers_DrawDebug3(uint32 colour, int32 x1, int32 y1, int32 x2, int32 y2);
void DrawHelpers_DrawDebug4(uint32 colour, int32 a2, int32 alpha, int32 x1, int32 y1, int32 x2, int32 y2);
void DrawHelpers_DrawDebug5(uint32 colour, int32 x, int32 y);

#endif //! OBJ_DRAWHELPERS_H
