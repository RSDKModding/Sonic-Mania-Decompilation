#ifndef OBJ_TTCUTSCENE_H
#define OBJ_TTCUTSCENE_H

#include "../../SonicMania.h"

//Object Class
class ObjectTTCutscene : public Object {
public:

};

//Entity Class
class EntityTTCutscene : public Entity {
public:

};

//Object Entity
ObjectTTCutscene TTCutscene;

//Entity Functions
void TTCutscene_Update();
void TTCutscene_EarlyUpdate();
void TTCutscene_LateUpdate();
void TTCutscene_Draw();
void TTCutscene_Setup(void* subtype);
void TTCutscene_StageLoad();
void TTCutscene_GetAttributes();

#endif //!OBJ_TTCUTSCENE_H
