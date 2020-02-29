#ifndef OBJ_FXFADE_H
#define OBJ_FXFADE_H

#include "../../SonicMania.h"

//Object Class
class ObjectFXFade : public Object {
public:

};

//Entity Class
class EntityFXFade : public Entity {
public:

};

//Object Entity
ObjectFXFade FXFade;

//Entity Functions
void FXFade_Update();
void FXFade_EarlyUpdate();
void FXFade_LateUpdate();
void FXFade_Draw();
void FXFade_Setup(void* subtype);
void FXFade_StageLoad();
void FXFade_GetAttributes();

#endif //!OBJ_FXFADE_H
