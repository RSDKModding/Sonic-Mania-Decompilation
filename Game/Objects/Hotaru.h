#ifndef OBJ_HOTARU_H
#define OBJ_HOTARU_H

#include "../../SonicMania.h"

//Object Class
class ObjectHotaru : public Object {
public:

};

//Entity Class
class EntityHotaru : public Entity {
public:

};

//Object Entity
ObjectHotaru Hotaru;

//Entity Functions
void Hotaru_Update();
void Hotaru_EarlyUpdate();
void Hotaru_LateUpdate();
void Hotaru_Draw();
void Hotaru_Setup(void* subtype);
void Hotaru_StageLoad();
void Hotaru_GetAttributes();

#endif //!OBJ_HOTARU_H
