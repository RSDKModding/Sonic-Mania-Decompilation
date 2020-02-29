#ifndef OBJ_TMZ1OUTRO_H
#define OBJ_TMZ1OUTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectTMZ1Outro : public Object {
public:

};

//Entity Class
class EntityTMZ1Outro : public Entity {
public:

};

//Object Entity
ObjectTMZ1Outro TMZ1Outro;

//Entity Functions
void TMZ1Outro_Update();
void TMZ1Outro_EarlyUpdate();
void TMZ1Outro_LateUpdate();
void TMZ1Outro_Draw();
void TMZ1Outro_Setup(void* subtype);
void TMZ1Outro_StageLoad();
void TMZ1Outro_GetAttributes();

#endif //!OBJ_TMZ1OUTRO_H
