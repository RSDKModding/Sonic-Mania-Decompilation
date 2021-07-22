#ifndef OBJ_DASETUP_H
#define OBJ_DASETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectDASetup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDASetup;

// Object Struct
extern ObjectDASetup *DASetup;

// Standard Entity Events
void DASetup_Update(void);
void DASetup_LateUpdate(void);
void DASetup_StaticUpdate(void);
void DASetup_Draw(void);
void DASetup_Create(void* data);
void DASetup_StageLoad(void);
void DASetup_EditorDraw(void);
void DASetup_EditorLoad(void);
void DASetup_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_DASETUP_H
