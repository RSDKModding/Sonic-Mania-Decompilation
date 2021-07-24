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
void DrawHelpers_DrawHitboxOutline(uint colour, byte dir, int x, int y, Hitbox *hitbox);
void DrawHelpers_DrawDebug3(uint colour, int x1, int y1, int x2, int y2);
void DrawHelpers_DrawDebug4(uint colour, int a2, int alpha, int x1, int y1, int x2, int y2);
void DrawHelpers_DrawDebug5(uint colour, int x, int y);

#endif //! OBJ_DRAWHELPERS_H
