#ifndef OBJ_OOZ1OUTRO_H
#define OBJ_OOZ1OUTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectOOZ1Outro : public Object {
public:

};

//Entity Class
class EntityOOZ1Outro : public Entity {
public:

};

//Object Entity
ObjectOOZ1Outro OOZ1Outro;

//Entity Functions
void OOZ1Outro_Update();
void OOZ1Outro_EarlyUpdate();
void OOZ1Outro_LateUpdate();
void OOZ1Outro_Draw();
void OOZ1Outro_Setup(void* subtype);
void OOZ1Outro_StageLoad();
void OOZ1Outro_GetAttributes();

#endif //!OBJ_OOZ1OUTRO_H
