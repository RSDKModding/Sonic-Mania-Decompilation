#ifndef OBJ_TAEMERALD_H
#define OBJ_TAEMERALD_H

//Object Class
class TAEmerald : Object {

};

//Entity Class
class EntityTAEmerald : Entity {

};

//Entity Functions
void TAEmerald_Update();
void TAEmerald_EarlyUpdate();
void TAEmerald_LateUpdate();
void TAEmerald_Draw();
void TAEmerald_Setup(void* subtype);
void TAEmerald_StageLoad();
void TAEmerald_GetAttributes();

#endif //!OBJ_TAEMERALD_H
