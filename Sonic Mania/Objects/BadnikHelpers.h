#ifndef OBJ_BADNIKHELPERS_H
#define OBJ_BADNIKHELPERS_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBadnikHelpers;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBadnikHelpers;

// Object Entity
extern ObjectBadnikHelpers *BadnikHelpers;

// Standard Entity Events
void BadnikHelpers_Update();
void BadnikHelpers_LateUpdate();
void BadnikHelpers_StaticUpdate();
void BadnikHelpers_Draw();
void BadnikHelpers_Create(void* data);
void BadnikHelpers_StageLoad();
void BadnikHelpers_EditorDraw();
void BadnikHelpers_EditorLoad();
void BadnikHelpers_Serialize();

// Extra Entity Functions


#endif //!OBJ_BADNIKHELPERS_H
