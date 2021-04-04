#ifndef OBJ_MSFACTORY_H
#define OBJ_MSFACTORY_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMSFactory;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMSFactory;

// Object Struct
extern ObjectMSFactory *MSFactory;

// Standard Entity Events
void MSFactory_Update(void);
void MSFactory_LateUpdate(void);
void MSFactory_StaticUpdate(void);
void MSFactory_Draw(void);
void MSFactory_Create(void* data);
void MSFactory_StageLoad(void);
void MSFactory_EditorDraw(void);
void MSFactory_EditorLoad(void);
void MSFactory_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MSFACTORY_H
