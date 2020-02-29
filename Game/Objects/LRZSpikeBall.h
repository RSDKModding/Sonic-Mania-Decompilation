#ifndef OBJ_LRZSPIKEBALL_H
#define OBJ_LRZSPIKEBALL_H

#include "../../SonicMania.h"

//Object Class
class ObjectLRZSpikeBall : public Object {
public:

};

//Entity Class
class EntityLRZSpikeBall : public Entity {
public:

};

//Object Entity
ObjectLRZSpikeBall LRZSpikeBall;

//Entity Functions
void LRZSpikeBall_Update();
void LRZSpikeBall_EarlyUpdate();
void LRZSpikeBall_LateUpdate();
void LRZSpikeBall_Draw();
void LRZSpikeBall_Setup(void* subtype);
void LRZSpikeBall_StageLoad();
void LRZSpikeBall_GetAttributes();

#endif //!OBJ_LRZSPIKEBALL_H
