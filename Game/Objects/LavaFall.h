#ifndef OBJ_LAVAFALL_H
#define OBJ_LAVAFALL_H

#include "../../SonicMania.h"

//Object Class
class ObjectLavaFall : public Object {
public:

};

//Entity Class
class EntityLavaFall : public Entity {
public:

};

//Object Entity
ObjectLavaFall LavaFall;

//Entity Functions
void LavaFall_Update();
void LavaFall_EarlyUpdate();
void LavaFall_LateUpdate();
void LavaFall_Draw();
void LavaFall_Setup(void* subtype);
void LavaFall_StageLoad();
void LavaFall_GetAttributes();

#endif //!OBJ_LAVAFALL_H
