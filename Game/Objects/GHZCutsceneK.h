#ifndef OBJ_GHZCUTSCENEK_H
#define OBJ_GHZCUTSCENEK_H

#include "../../SonicMania.h"

//Object Class
class ObjectGHZCutsceneK : public Object {
public:

};

//Entity Class
class EntityGHZCutsceneK : public Entity {
public:

};

//Object Entity
ObjectGHZCutsceneK GHZCutsceneK;

//Entity Functions
void GHZCutsceneK_Update();
void GHZCutsceneK_EarlyUpdate();
void GHZCutsceneK_LateUpdate();
void GHZCutsceneK_Draw();
void GHZCutsceneK_Setup(void* subtype);
void GHZCutsceneK_StageLoad();
void GHZCutsceneK_GetAttributes();

#endif //!OBJ_GHZCUTSCENEK_H
