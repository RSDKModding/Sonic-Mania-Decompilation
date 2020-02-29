#ifndef OBJ_TMZ2OUTRO_H
#define OBJ_TMZ2OUTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectTMZ2Outro : public Object {
public:

};

//Entity Class
class EntityTMZ2Outro : public Entity {
public:

};

//Object Entity
ObjectTMZ2Outro TMZ2Outro;

//Entity Functions
void TMZ2Outro_Update();
void TMZ2Outro_EarlyUpdate();
void TMZ2Outro_LateUpdate();
void TMZ2Outro_Draw();
void TMZ2Outro_Setup(void* subtype);
void TMZ2Outro_StageLoad();
void TMZ2Outro_GetAttributes();

#endif //!OBJ_TMZ2OUTRO_H
