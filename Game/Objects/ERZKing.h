#ifndef OBJ_ERZKING_H
#define OBJ_ERZKING_H

//Object Class
class ERZKing : Object {

};

//Entity Class
class EntityERZKing : Entity {

};

//Entity Functions
void ERZKing_Update();
void ERZKing_EarlyUpdate();
void ERZKing_LateUpdate();
void ERZKing_Draw();
void ERZKing_Setup(void* subtype);
void ERZKing_StageLoad();
void ERZKing_GetAttributes();

#endif //!OBJ_ERZKING_H
