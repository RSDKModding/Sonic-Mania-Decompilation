#ifndef OBJ_COLLAPSINGSAND_H
#define OBJ_COLLAPSINGSAND_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCollapsingSand;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCollapsingSand;

// Object Struct
extern ObjectCollapsingSand *CollapsingSand;

// Standard Entity Events
void CollapsingSand_Update(void);
void CollapsingSand_LateUpdate(void);
void CollapsingSand_StaticUpdate(void);
void CollapsingSand_Draw(void);
void CollapsingSand_Create(void* data);
void CollapsingSand_StageLoad(void);
void CollapsingSand_EditorDraw(void);
void CollapsingSand_EditorLoad(void);
void CollapsingSand_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_COLLAPSINGSAND_H
