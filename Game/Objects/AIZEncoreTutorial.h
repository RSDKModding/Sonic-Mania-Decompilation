#ifndef OBJ_AIZENCORETUTORIAL_H
#define OBJ_AIZENCORETUTORIAL_H

#include "../../SonicMania.h"

//Object Class
class ObjectAIZEncoreTutorial : public Object {
public:

};

//Entity Class
class EntityAIZEncoreTutorial : public Entity {
public:

};

//Object Entity
ObjectAIZEncoreTutorial AIZEncoreTutorial;

//Entity Functions
void AIZEncoreTutorial_Update();
void AIZEncoreTutorial_EarlyUpdate();
void AIZEncoreTutorial_LateUpdate();
void AIZEncoreTutorial_Draw();
void AIZEncoreTutorial_Setup(void* subtype);
void AIZEncoreTutorial_StageLoad();
void AIZEncoreTutorial_GetAttributes();

#endif //!OBJ_AIZENCORETUTORIAL_H
