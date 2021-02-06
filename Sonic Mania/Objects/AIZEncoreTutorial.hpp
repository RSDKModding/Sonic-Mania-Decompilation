#ifndef OBJ_AIZENCORETUTORIAL_H
#define OBJ_AIZENCORETUTORIAL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectAIZEncoreTutorial : Object {

};

// Entity Class
struct EntityAIZEncoreTutorial : Entity {

};

// Object Entity
extern ObjectAIZEncoreTutorial AIZEncoreTutorial;

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


#endif //!OBJ_AIZENCORETUTORIAL_H
