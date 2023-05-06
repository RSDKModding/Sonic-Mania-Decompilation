#ifndef OBJ_DRAWHELPERS_H
#define OBJ_DRAWHELPERS_H

#include "Game.h"

// Object Class
struct ObjectDrawHelpers {
    RSDK_OBJECT
};

// Entity Class
struct EntityDrawHelpers {
    RSDK_ENTITY
};

// Object Entity
extern ObjectDrawHelpers *DrawHelpers;

// Standard Entity Events
void DrawHelpers_Update(void);
void DrawHelpers_LateUpdate(void);
void DrawHelpers_StaticUpdate(void);
void DrawHelpers_Draw(void);
void DrawHelpers_Create(void *data);
void DrawHelpers_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void DrawHelpers_EditorDraw(void);
void DrawHelpers_EditorLoad(void);
#endif
void DrawHelpers_Serialize(void);

// Extra Entity Functions
void DrawHelpers_DrawHitboxOutline(int32 x, int32 y, Hitbox *hitbox, uint8 direction, uint32 color);
void DrawHelpers_DrawArrowAdditive(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color);
void DrawHelpers_DrawIsocelesTriangle(int32 x1, int32 y1, int32 x2, int32 y2, int32 edgeSize, uint32 color, uint32 inkEffect, uint32 alpha);
void DrawHelpers_DrawCross(int32 x, int32 y, int32 sizeX, int32 sizeY, uint32 color);

// Custom Helpers I added for editor stuff
void DrawHelpers_DrawArrow(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color, uint32 inkEffect, uint32 alpha);
void DrawHelpers_DrawRectOutline(int32 x, int32 y, int32 sizeX, int32 sizeY, uint32 color);
void DrawHelpers_DrawArenaBounds(int32 left, int32 top, int32 right, int32 bottom, uint8 sideMasks, uint32 color);

#endif //! OBJ_DRAWHELPERS_H
