#ifndef OBJ_TRANSPORTTUBE_H
#define OBJ_TRANSPORTTUBE_H

#include "../../SonicMania.h"

//Object Class
class ObjectTransportTube : public Object {
public:

};

//Entity Class
class EntityTransportTube : public Entity {
public:

};

//Object Entity
ObjectTransportTube TransportTube;

//Entity Functions
void TransportTube_Update();
void TransportTube_EarlyUpdate();
void TransportTube_LateUpdate();
void TransportTube_Draw();
void TransportTube_Setup(void* subtype);
void TransportTube_StageLoad();
void TransportTube_GetAttributes();

#endif //!OBJ_TRANSPORTTUBE_H
