#ifndef OBJ_TRYAGAINE_H
#define OBJ_TRYAGAINE_H

#include "../../SonicMania.h"

//Object Class
class ObjectTryAgainE : public Object {
public:

};

//Entity Class
class EntityTryAgainE : public Entity {
public:

};

//Object Entity
ObjectTryAgainE TryAgainE;

//Entity Functions
void TryAgainE_Update();
void TryAgainE_EarlyUpdate();
void TryAgainE_LateUpdate();
void TryAgainE_Draw();
void TryAgainE_Setup(void* subtype);
void TryAgainE_StageLoad();
void TryAgainE_GetAttributes();

#endif //!OBJ_TRYAGAINE_H
