#ifndef OBJ_SPINSIGN_H
#define OBJ_SPINSIGN_H

#include "../../SonicMania.h"

//Object Class
class ObjectSpinSign : public Object {
public:

};

//Entity Class
class EntitySpinSign : public Entity {
public:

};

//Object Entity
ObjectSpinSign SpinSign;

//Entity Functions
void SpinSign_Update();
void SpinSign_EarlyUpdate();
void SpinSign_LateUpdate();
void SpinSign_Draw();
void SpinSign_Setup(void* subtype);
void SpinSign_StageLoad();
void SpinSign_GetAttributes();

#endif //!OBJ_SPINSIGN_H
