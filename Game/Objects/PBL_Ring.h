#ifndef OBJ_PBL_RING_H
#define OBJ_PBL_RING_H

//Object Class
class PBL_Ring : Object {

};

//Entity Class
class EntityPBL_Ring : Entity {

};

//Entity Functions
void PBL_Ring_Update();
void PBL_Ring_EarlyUpdate();
void PBL_Ring_LateUpdate();
void PBL_Ring_Draw();
void PBL_Ring_Setup(void* subtype);
void PBL_Ring_StageLoad();
void PBL_Ring_GetAttributes();

#endif //!OBJ_PBL_RING_H
