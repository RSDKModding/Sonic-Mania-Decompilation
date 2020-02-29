#ifndef OBJ_SPZ2OUTRO_H
#define OBJ_SPZ2OUTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectSPZ2Outro : public Object {
public:

};

//Entity Class
class EntitySPZ2Outro : public Entity {
public:

};

//Object Entity
ObjectSPZ2Outro SPZ2Outro;

//Entity Functions
void SPZ2Outro_Update();
void SPZ2Outro_EarlyUpdate();
void SPZ2Outro_LateUpdate();
void SPZ2Outro_Draw();
void SPZ2Outro_Setup(void* subtype);
void SPZ2Outro_StageLoad();
void SPZ2Outro_GetAttributes();

#endif //!OBJ_SPZ2OUTRO_H
