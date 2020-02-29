#ifndef OBJ_MSZCUTSCENEK_H
#define OBJ_MSZCUTSCENEK_H

#include "../../SonicMania.h"

//Object Class
class ObjectMSZCutsceneK : public Object {
public:

};

//Entity Class
class EntityMSZCutsceneK : public Entity {
public:

};

//Object Entity
ObjectMSZCutsceneK MSZCutsceneK;

//Entity Functions
void MSZCutsceneK_Update();
void MSZCutsceneK_EarlyUpdate();
void MSZCutsceneK_LateUpdate();
void MSZCutsceneK_Draw();
void MSZCutsceneK_Setup(void* subtype);
void MSZCutsceneK_StageLoad();
void MSZCutsceneK_GetAttributes();

#endif //!OBJ_MSZCUTSCENEK_H
