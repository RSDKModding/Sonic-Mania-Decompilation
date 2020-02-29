#ifndef OBJ_GHZ2OUTRO_H
#define OBJ_GHZ2OUTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectGHZ2Outro : public Object {
public:

};

//Entity Class
class EntityGHZ2Outro : public Entity {
public:

};

//Object Entity
ObjectGHZ2Outro GHZ2Outro;

//Entity Functions
void GHZ2Outro_Update();
void GHZ2Outro_EarlyUpdate();
void GHZ2Outro_LateUpdate();
void GHZ2Outro_Draw();
void GHZ2Outro_Setup(void* subtype);
void GHZ2Outro_StageLoad();
void GHZ2Outro_GetAttributes();

#endif //!OBJ_GHZ2OUTRO_H
