#ifndef OBJ_HPZEMERALD_H
#define OBJ_HPZEMERALD_H

#include "../../SonicMania.h"

//Object Class
class ObjectHPZEmerald : public Object {
public:

};

//Entity Class
class EntityHPZEmerald : public Entity {
public:

};

//Object Entity
ObjectHPZEmerald HPZEmerald;

//Entity Functions
void HPZEmerald_Update();
void HPZEmerald_EarlyUpdate();
void HPZEmerald_LateUpdate();
void HPZEmerald_Draw();
void HPZEmerald_Setup(void* subtype);
void HPZEmerald_StageLoad();
void HPZEmerald_GetAttributes();

#endif //!OBJ_HPZEMERALD_H
