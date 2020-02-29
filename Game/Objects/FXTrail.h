#ifndef OBJ_FXTRAIL_H
#define OBJ_FXTRAIL_H

//Object Class
class FXTrail : Object {

};

//Entity Class
class EntityFXTrail : Entity {

};

//Entity Functions
void FXTrail_Update();
void FXTrail_EarlyUpdate();
void FXTrail_LateUpdate();
void FXTrail_Draw();
void FXTrail_Setup(void* subtype);
void FXTrail_StageLoad();
void FXTrail_GetAttributes();

#endif //!OBJ_FXTRAIL_H
