#ifndef OBJ_CABLEWARP_H
#define OBJ_CABLEWARP_H

#include "../../SonicMania.h"

//Object Class
class ObjectCableWarp : public Object {
public:

};

//Entity Class
class EntityCableWarp : public Entity {
public:

};

//Object Entity
ObjectCableWarp CableWarp;

//Entity Functions
void CableWarp_Update();
void CableWarp_EarlyUpdate();
void CableWarp_LateUpdate();
void CableWarp_Draw();
void CableWarp_Setup(void* subtype);
void CableWarp_StageLoad();
void CableWarp_GetAttributes();

#endif //!OBJ_CABLEWARP_H
