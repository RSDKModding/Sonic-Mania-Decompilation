#ifndef OBJ_SSZ1OUTRO_H
#define OBJ_SSZ1OUTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectSSZ1Outro : public Object {
public:

};

//Entity Class
class EntitySSZ1Outro : public Entity {
public:

};

//Object Entity
ObjectSSZ1Outro SSZ1Outro;

//Entity Functions
void SSZ1Outro_Update();
void SSZ1Outro_EarlyUpdate();
void SSZ1Outro_LateUpdate();
void SSZ1Outro_Draw();
void SSZ1Outro_Setup(void* subtype);
void SSZ1Outro_StageLoad();
void SSZ1Outro_GetAttributes();

#endif //!OBJ_SSZ1OUTRO_H
