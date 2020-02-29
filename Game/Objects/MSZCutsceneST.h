#ifndef OBJ_MSZCUTSCENEST_H
#define OBJ_MSZCUTSCENEST_H

#include "../../SonicMania.h"

//Object Class
class ObjectMSZCutsceneST : public Object {
public:

};

//Entity Class
class EntityMSZCutsceneST : public Entity {
public:

};

//Object Entity
ObjectMSZCutsceneST MSZCutsceneST;

//Entity Functions
void MSZCutsceneST_Update();
void MSZCutsceneST_EarlyUpdate();
void MSZCutsceneST_LateUpdate();
void MSZCutsceneST_Draw();
void MSZCutsceneST_Setup(void* subtype);
void MSZCutsceneST_StageLoad();
void MSZCutsceneST_GetAttributes();

#endif //!OBJ_MSZCUTSCENEST_H
