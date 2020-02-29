#ifndef OBJ_FXEXPANDRING_H
#define OBJ_FXEXPANDRING_H

//Object Class
class FXExpandRing : Object {

};

//Entity Class
class EntityFXExpandRing : Entity {

};

//Entity Functions
void FXExpandRing_Update();
void FXExpandRing_EarlyUpdate();
void FXExpandRing_LateUpdate();
void FXExpandRing_Draw();
void FXExpandRing_Setup(void* subtype);
void FXExpandRing_StageLoad();
void FXExpandRing_GetAttributes();

#endif //!OBJ_FXEXPANDRING_H
