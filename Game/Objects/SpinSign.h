#ifndef OBJ_SPINSIGN_H
#define OBJ_SPINSIGN_H

//Object Class
class SpinSign : Object {

};

//Entity Class
class EntitySpinSign : Entity {

};

//Entity Functions
void SpinSign_Update();
void SpinSign_EarlyUpdate();
void SpinSign_LateUpdate();
void SpinSign_Draw();
void SpinSign_Setup(void* subtype);
void SpinSign_StageLoad();
void SpinSign_GetAttributes();

#endif //!OBJ_SPINSIGN_H
