#ifndef OBJ_PULLCHAIN_H
#define OBJ_PULLCHAIN_H

#include "../../SonicMania.h"

//Object Class
class ObjectPullChain : public Object {
public:

};

//Entity Class
class EntityPullChain : public Entity {
public:

};

//Object Entity
ObjectPullChain PullChain;

//Entity Functions
void PullChain_Update();
void PullChain_EarlyUpdate();
void PullChain_LateUpdate();
void PullChain_Draw();
void PullChain_Setup(void* subtype);
void PullChain_StageLoad();
void PullChain_GetAttributes();

#endif //!OBJ_PULLCHAIN_H
