#ifndef OBJ_CHAOSEMERALD_H
#define OBJ_CHAOSEMERALD_H

#include "../../SonicMania.h"

//Object Class
class ObjectChaosEmerald : public Object {
public:

};

//Entity Class
class EntityChaosEmerald : public Entity {
public:

};

//Object Entity
ObjectChaosEmerald ChaosEmerald;

//Entity Functions
void ChaosEmerald_Update();
void ChaosEmerald_EarlyUpdate();
void ChaosEmerald_LateUpdate();
void ChaosEmerald_Draw();
void ChaosEmerald_Setup(void* subtype);
void ChaosEmerald_StageLoad();
void ChaosEmerald_GetAttributes();

#endif //!OBJ_CHAOSEMERALD_H
