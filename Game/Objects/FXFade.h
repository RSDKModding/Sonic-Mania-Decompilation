#ifndef OBJ_FXFADE_H
#define OBJ_FXFADE_H

//Object Class
class FXFade : Object {

};

//Entity Class
class EntityFXFade : Entity {

};

//Entity Functions
void FXFade_Update();
void FXFade_EarlyUpdate();
void FXFade_LateUpdate();
void FXFade_Draw();
void FXFade_Setup(void* subtype);
void FXFade_StageLoad();
void FXFade_GetAttributes();

#endif //!OBJ_FXFADE_H
