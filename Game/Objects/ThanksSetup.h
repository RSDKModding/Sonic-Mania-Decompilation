#ifndef OBJ_THANKSSETUP_H
#define OBJ_THANKSSETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectThanksSetup : public Object {
public:

};

//Entity Class
class EntityThanksSetup : public Entity {
public:

};

//Object Entity
ObjectThanksSetup ThanksSetup;

//Entity Functions
void ThanksSetup_Update();
void ThanksSetup_EarlyUpdate();
void ThanksSetup_LateUpdate();
void ThanksSetup_Draw();
void ThanksSetup_Setup(void* subtype);
void ThanksSetup_StageLoad();
void ThanksSetup_GetAttributes();

#endif //!OBJ_THANKSSETUP_H
