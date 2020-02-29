#ifndef OBJ_MSORB_H
#define OBJ_MSORB_H

//Object Class
class MSOrb : Object {

};

//Entity Class
class EntityMSOrb : Entity {

};

//Entity Functions
void MSOrb_Update();
void MSOrb_EarlyUpdate();
void MSOrb_LateUpdate();
void MSOrb_Draw();
void MSOrb_Setup(void* subtype);
void MSOrb_StageLoad();
void MSOrb_GetAttributes();

#endif //!OBJ_MSORB_H
