#ifndef OBJ_RPLANESHIFTER_H
#define OBJ_RPLANESHIFTER_H

//Object Class
class RPlaneShifter : Object {

};

//Entity Class
class EntityRPlaneShifter : Entity {

};

//Entity Functions
void RPlaneShifter_Update();
void RPlaneShifter_EarlyUpdate();
void RPlaneShifter_LateUpdate();
void RPlaneShifter_Draw();
void RPlaneShifter_Setup(void* subtype);
void RPlaneShifter_StageLoad();
void RPlaneShifter_GetAttributes();

#endif //!OBJ_RPLANESHIFTER_H
