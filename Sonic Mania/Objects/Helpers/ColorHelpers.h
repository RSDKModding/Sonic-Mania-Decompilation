#ifndef OBJ_COLORHELPERS_H
#define OBJ_COLORHELPERS_H

#include "SonicMania.h"

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
void ColorHelpers_Update(void);
void ColorHelpers_LateUpdate(void);
void ColorHelpers_StaticUpdate(void);
void ColorHelpers_Draw(void);
void ColorHelpers_Create(void* data);
void ColorHelpers_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void ColorHelpers_EditorDraw(void);
void ColorHelpers_EditorLoad(void);
#endif
void ColorHelpers_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_COLORHELPERS_H
