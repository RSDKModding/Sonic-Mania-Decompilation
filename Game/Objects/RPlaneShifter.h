#ifndef OBJ_RPLANESHIFTER_H
#define OBJ_RPLANESHIFTER_H

#include "../../SonicMania.h"

//Object Class
class ObjectRPlaneShifter : public Object {
public:

};

//Entity Class
class EntityRPlaneShifter : public Entity {
public:

};

//Object Entity
ObjectRPlaneShifter RPlaneShifter;

//Entity Functions
void RPlaneShifter_Update();
void RPlaneShifter_EarlyUpdate();
void RPlaneShifter_LateUpdate();
void RPlaneShifter_Draw();
void RPlaneShifter_Setup(void* subtype);
void RPlaneShifter_StageLoad();
void RPlaneShifter_GetAttributes();

#endif //!OBJ_RPLANESHIFTER_H
