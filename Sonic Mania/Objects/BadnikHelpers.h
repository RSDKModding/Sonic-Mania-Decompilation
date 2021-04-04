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
void BadnikHelpers_Update(void);
void BadnikHelpers_LateUpdate(void);
void BadnikHelpers_StaticUpdate(void);
void BadnikHelpers_Draw(void);
void BadnikHelpers_Create(void* data);
void BadnikHelpers_StageLoad(void);
void BadnikHelpers_EditorDraw(void);
void BadnikHelpers_EditorLoad(void);
void BadnikHelpers_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BADNIKHELPERS_H
