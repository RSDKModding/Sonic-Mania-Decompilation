#ifndef OBJ_DCEVENT_H
#define OBJ_DCEVENT_H

#include "../../SonicMania.h"

//Object Class
class ObjectDCEvent : public Object {
public:

};

//Entity Class
class EntityDCEvent : public Entity {
public:

};

//Object Entity
ObjectDCEvent DCEvent;

//Entity Functions
void DCEvent_Update();
void DCEvent_EarlyUpdate();
void DCEvent_LateUpdate();
void DCEvent_Draw();
void DCEvent_Setup(void* subtype);
void DCEvent_StageLoad();
void DCEvent_GetAttributes();

#endif //!OBJ_DCEVENT_H
