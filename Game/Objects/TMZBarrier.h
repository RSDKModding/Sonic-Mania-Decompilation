#ifndef OBJ_TMZBARRIER_H
#define OBJ_TMZBARRIER_H

#include "../../SonicMania.h"

//Object Class
class ObjectTMZBarrier : public Object {
public:

};

//Entity Class
class EntityTMZBarrier : public Entity {
public:

};

//Object Entity
ObjectTMZBarrier TMZBarrier;

//Entity Functions
void TMZBarrier_Update();
void TMZBarrier_EarlyUpdate();
void TMZBarrier_LateUpdate();
void TMZBarrier_Draw();
void TMZBarrier_Setup(void* subtype);
void TMZBarrier_StageLoad();
void TMZBarrier_GetAttributes();

#endif //!OBJ_TMZBARRIER_H
