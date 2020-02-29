#ifndef OBJ_PHANTOMSHINOBI_H
#define OBJ_PHANTOMSHINOBI_H

//Object Class
class PhantomShinobi : Object {

};

//Entity Class
class EntityPhantomShinobi : Entity {

};

//Entity Functions
void PhantomShinobi_Update();
void PhantomShinobi_EarlyUpdate();
void PhantomShinobi_LateUpdate();
void PhantomShinobi_Draw();
void PhantomShinobi_Setup(void* subtype);
void PhantomShinobi_StageLoad();
void PhantomShinobi_GetAttributes();

#endif //!OBJ_PHANTOMSHINOBI_H
