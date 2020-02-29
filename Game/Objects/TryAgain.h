#ifndef OBJ_TRYAGAIN_H
#define OBJ_TRYAGAIN_H

#include "../../SonicMania.h"

//Object Class
class ObjectTryAgain : public Object {
public:

};

//Entity Class
class EntityTryAgain : public Entity {
public:

};

//Object Entity
ObjectTryAgain TryAgain;

//Entity Functions
void TryAgain_Update();
void TryAgain_EarlyUpdate();
void TryAgain_LateUpdate();
void TryAgain_Draw();
void TryAgain_Setup(void* subtype);
void TryAgain_StageLoad();
void TryAgain_GetAttributes();

#endif //!OBJ_TRYAGAIN_H
