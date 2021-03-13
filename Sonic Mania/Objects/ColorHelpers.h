#ifndef OBJ_COLORHELPERS_H
#define OBJ_COLORHELPERS_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectColorHelpers;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityColorHelpers;

// Object Entity
extern ObjectColorHelpers *ColorHelpers;

// Standard Entity Events
void ColorHelpers_Update();
void ColorHelpers_LateUpdate();
void ColorHelpers_StaticUpdate();
void ColorHelpers_Draw();
void ColorHelpers_Create(void* data);
void ColorHelpers_StageLoad();
void ColorHelpers_EditorDraw();
void ColorHelpers_EditorLoad();
void ColorHelpers_Serialize();

// Extra Entity Functions


#endif //!OBJ_COLORHELPERS_H
