#ifndef OBJ_MMZ2OUTRO_H
#define OBJ_MMZ2OUTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectMMZ2Outro : public Object {
public:

};

//Entity Class
class EntityMMZ2Outro : public Entity {
public:

};

//Object Entity
ObjectMMZ2Outro MMZ2Outro;

//Entity Functions
void MMZ2Outro_Update();
void MMZ2Outro_EarlyUpdate();
void MMZ2Outro_LateUpdate();
void MMZ2Outro_Draw();
void MMZ2Outro_Setup(void* subtype);
void MMZ2Outro_StageLoad();
void MMZ2Outro_GetAttributes();

#endif //!OBJ_MMZ2OUTRO_H
