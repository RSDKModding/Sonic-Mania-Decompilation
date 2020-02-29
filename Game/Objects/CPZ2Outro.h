#ifndef OBJ_CPZ2OUTRO_H
#define OBJ_CPZ2OUTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectCPZ2Outro : public Object {
public:

};

//Entity Class
class EntityCPZ2Outro : public Entity {
public:

};

//Object Entity
ObjectCPZ2Outro CPZ2Outro;

//Entity Functions
void CPZ2Outro_Update();
void CPZ2Outro_EarlyUpdate();
void CPZ2Outro_LateUpdate();
void CPZ2Outro_Draw();
void CPZ2Outro_Setup(void* subtype);
void CPZ2Outro_StageLoad();
void CPZ2Outro_GetAttributes();

#endif //!OBJ_CPZ2OUTRO_H
