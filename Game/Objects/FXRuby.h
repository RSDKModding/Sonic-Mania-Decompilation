#ifndef OBJ_FXRUBY_H
#define OBJ_FXRUBY_H

#include "../../SonicMania.h"

//Object Class
class ObjectFXRuby : public Object {
public:

};

//Entity Class
class EntityFXRuby : public Entity {
public:

};

//Object Entity
ObjectFXRuby FXRuby;

//Entity Functions
void FXRuby_Update();
void FXRuby_EarlyUpdate();
void FXRuby_LateUpdate();
void FXRuby_Draw();
void FXRuby_Setup(void* subtype);
void FXRuby_StageLoad();
void FXRuby_GetAttributes();

#endif //!OBJ_FXRUBY_H
