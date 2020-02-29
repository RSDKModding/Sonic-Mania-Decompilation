#ifndef OBJ_MONKEYDUDE_H
#define OBJ_MONKEYDUDE_H

#include "../../SonicMania.h"

//Object Class
class ObjectMonkeyDude : public Object {
public:

};

//Entity Class
class EntityMonkeyDude : public Entity {
public:

};

//Object Entity
ObjectMonkeyDude MonkeyDude;

//Entity Functions
void MonkeyDude_Update();
void MonkeyDude_EarlyUpdate();
void MonkeyDude_LateUpdate();
void MonkeyDude_Draw();
void MonkeyDude_Setup(void* subtype);
void MonkeyDude_StageLoad();
void MonkeyDude_GetAttributes();

#endif //!OBJ_MONKEYDUDE_H
