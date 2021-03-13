#ifndef OBJ_DRAWHELPERS_H
#define OBJ_DRAWHELPERS_H

#include "../SonicMania.h"

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
void DrawHelpers_Update();
void DrawHelpers_LateUpdate();
void DrawHelpers_StaticUpdate();
void DrawHelpers_Draw();
void DrawHelpers_Create(void* data);
void DrawHelpers_StageLoad();
void DrawHelpers_EditorDraw();
void DrawHelpers_EditorLoad();
void DrawHelpers_Serialize();

// Extra Entity Functions


#endif //!OBJ_DRAWHELPERS_H
