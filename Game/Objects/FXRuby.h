#ifndef OBJ_FXRUBY_H
#define OBJ_FXRUBY_H

//Object Class
class FXRuby : Object {

};

//Entity Class
class EntityFXRuby : Entity {

};

//Entity Functions
void FXRuby_Update();
void FXRuby_EarlyUpdate();
void FXRuby_LateUpdate();
void FXRuby_Draw();
void FXRuby_Setup(void* subtype);
void FXRuby_StageLoad();
void FXRuby_GetAttributes();

#endif //!OBJ_FXRUBY_H
