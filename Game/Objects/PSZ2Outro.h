#ifndef OBJ_PSZ2OUTRO_H
#define OBJ_PSZ2OUTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectPSZ2Outro : public Object {
public:

};

//Entity Class
class EntityPSZ2Outro : public Entity {
public:

};

//Object Entity
ObjectPSZ2Outro PSZ2Outro;

//Entity Functions
void PSZ2Outro_Update();
void PSZ2Outro_EarlyUpdate();
void PSZ2Outro_LateUpdate();
void PSZ2Outro_Draw();
void PSZ2Outro_Setup(void* subtype);
void PSZ2Outro_StageLoad();
void PSZ2Outro_GetAttributes();

#endif //!OBJ_PSZ2OUTRO_H
