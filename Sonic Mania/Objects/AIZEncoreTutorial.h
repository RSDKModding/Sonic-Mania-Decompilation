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
void AIZEncoreTutorial_Update();
void AIZEncoreTutorial_LateUpdate();
void AIZEncoreTutorial_StaticUpdate();
void AIZEncoreTutorial_Draw();
void AIZEncoreTutorial_Create(void* data);
void AIZEncoreTutorial_StageLoad();
void AIZEncoreTutorial_EditorDraw();
void AIZEncoreTutorial_EditorLoad();
void AIZEncoreTutorial_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_AIZENCORETUTORIAL_H
