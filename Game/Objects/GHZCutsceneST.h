#ifndef OBJ_GHZCUTSCENEST_H
#define OBJ_GHZCUTSCENEST_H

#include "../../SonicMania.h"

//Object Class
class ObjectGHZCutsceneST : public Object {
public:

};

//Entity Class
class EntityGHZCutsceneST : public Entity {
public:

};

//Object Entity
ObjectGHZCutsceneST GHZCutsceneST;

//Entity Functions
void GHZCutsceneST_Update();
void GHZCutsceneST_EarlyUpdate();
void GHZCutsceneST_LateUpdate();
void GHZCutsceneST_Draw();
void GHZCutsceneST_Setup(void* subtype);
void GHZCutsceneST_StageLoad();
void GHZCutsceneST_GetAttributes();

#endif //!OBJ_GHZCUTSCENEST_H
