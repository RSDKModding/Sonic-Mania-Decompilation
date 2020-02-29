#ifndef OBJ_PLANESEESAW_H
#define OBJ_PLANESEESAW_H

//Object Class
class PlaneSeeSaw : Object {

};

//Entity Class
class EntityPlaneSeeSaw : Entity {

};

//Entity Functions
void PlaneSeeSaw_Update();
void PlaneSeeSaw_EarlyUpdate();
void PlaneSeeSaw_LateUpdate();
void PlaneSeeSaw_Draw();
void PlaneSeeSaw_Setup(void* subtype);
void PlaneSeeSaw_StageLoad();
void PlaneSeeSaw_GetAttributes();

#endif //!OBJ_PLANESEESAW_H
