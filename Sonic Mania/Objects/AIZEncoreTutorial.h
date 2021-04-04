#ifndef OBJ_AIZENCORETUTORIAL_H
#define OBJ_AIZENCORETUTORIAL_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectAIZEncoreTutorial;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityAIZEncoreTutorial;

// Object Struct
extern ObjectAIZEncoreTutorial *AIZEncoreTutorial;

// Standard Entity Events
void AIZEncoreTutorial_Update(void);
void AIZEncoreTutorial_LateUpdate(void);
void AIZEncoreTutorial_StaticUpdate(void);
void AIZEncoreTutorial_Draw(void);
void AIZEncoreTutorial_Create(void* data);
void AIZEncoreTutorial_StageLoad(void);
void AIZEncoreTutorial_EditorDraw(void);
void AIZEncoreTutorial_EditorLoad(void);
void AIZEncoreTutorial_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_AIZENCORETUTORIAL_H
