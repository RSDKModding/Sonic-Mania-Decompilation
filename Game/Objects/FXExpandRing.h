#ifndef OBJ_FXEXPANDRING_H
#define OBJ_FXEXPANDRING_H

#include "../../SonicMania.h"

//Object Class
class ObjectFXExpandRing : public Object {
public:

};

//Entity Class
class EntityFXExpandRing : public Entity {
public:

};

//Object Entity
ObjectFXExpandRing FXExpandRing;

//Entity Functions
void FXExpandRing_Update();
void FXExpandRing_EarlyUpdate();
void FXExpandRing_LateUpdate();
void FXExpandRing_Draw();
void FXExpandRing_Setup(void* subtype);
void FXExpandRing_StageLoad();
void FXExpandRing_GetAttributes();

#endif //!OBJ_FXEXPANDRING_H
