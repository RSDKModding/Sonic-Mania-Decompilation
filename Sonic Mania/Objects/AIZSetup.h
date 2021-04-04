#ifndef OBJ_AIZSETUP_H
#define OBJ_AIZSETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectAIZSetup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityAIZSetup;

// Object Struct
extern ObjectAIZSetup *AIZSetup;

// Standard Entity Events
void AIZSetup_Update(void);
void AIZSetup_LateUpdate(void);
void AIZSetup_StaticUpdate(void);
void AIZSetup_Draw(void);
void AIZSetup_Create(void* data);
void AIZSetup_StageLoad(void);
void AIZSetup_EditorDraw(void);
void AIZSetup_EditorLoad(void);
void AIZSetup_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_AIZSETUP_H
