#ifndef OBJ_FXTRAIL_H
#define OBJ_FXTRAIL_H

#include "../../SonicMania.h"

//Object Class
class ObjectFXTrail : public Object {
public:

};

//Entity Class
class EntityFXTrail : public Entity {
public:

};

//Object Entity
ObjectFXTrail FXTrail;

//Entity Functions
void FXTrail_Update();
void FXTrail_EarlyUpdate();
void FXTrail_LateUpdate();
void FXTrail_Draw();
void FXTrail_Setup(void* subtype);
void FXTrail_StageLoad();
void FXTrail_GetAttributes();

#endif //!OBJ_FXTRAIL_H
