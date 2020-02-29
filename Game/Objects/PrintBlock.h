#ifndef OBJ_PRINTBLOCK_H
#define OBJ_PRINTBLOCK_H

//Object Class
class PrintBlock : Object {

};

//Entity Class
class EntityPrintBlock : Entity {

};

//Entity Functions
void PrintBlock_Update();
void PrintBlock_EarlyUpdate();
void PrintBlock_LateUpdate();
void PrintBlock_Draw();
void PrintBlock_Setup(void* subtype);
void PrintBlock_StageLoad();
void PrintBlock_GetAttributes();

#endif //!OBJ_PRINTBLOCK_H
