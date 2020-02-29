#ifndef OBJ_OOZ2OUTRO_H
#define OBJ_OOZ2OUTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectOOZ2Outro : public Object {
public:

};

//Entity Class
class EntityOOZ2Outro : public Entity {
public:

};

//Object Entity
ObjectOOZ2Outro OOZ2Outro;

//Entity Functions
void OOZ2Outro_Update();
void OOZ2Outro_EarlyUpdate();
void OOZ2Outro_LateUpdate();
void OOZ2Outro_Draw();
void OOZ2Outro_Setup(void* subtype);
void OOZ2Outro_StageLoad();
void OOZ2Outro_GetAttributes();

#endif //!OBJ_OOZ2OUTRO_H
