#ifndef OBJ_MONKEYDUDE_H
#define OBJ_MONKEYDUDE_H

//Object Class
class MonkeyDude : Object {

};

//Entity Class
class EntityMonkeyDude : Entity {

};

//Entity Functions
void MonkeyDude_Update();
void MonkeyDude_EarlyUpdate();
void MonkeyDude_LateUpdate();
void MonkeyDude_Draw();
void MonkeyDude_Setup(void* subtype);
void MonkeyDude_StageLoad();
void MonkeyDude_GetAttributes();

#endif //!OBJ_MONKEYDUDE_H
