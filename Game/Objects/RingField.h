#ifndef OBJ_RINGFIELD_H
#define OBJ_RINGFIELD_H

//Object Class
class RingField : Object {

};

//Entity Class
class EntityRingField : Entity {

};

//Entity Functions
void RingField_Update();
void RingField_EarlyUpdate();
void RingField_LateUpdate();
void RingField_Draw();
void RingField_Setup(void* subtype);
void RingField_StageLoad();
void RingField_GetAttributes();

#endif //!OBJ_RINGFIELD_H
