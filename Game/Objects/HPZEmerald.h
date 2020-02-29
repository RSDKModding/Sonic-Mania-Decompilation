#ifndef OBJ_HPZEMERALD_H
#define OBJ_HPZEMERALD_H

//Object Class
class HPZEmerald : Object {

};

//Entity Class
class EntityHPZEmerald : Entity {

};

//Entity Functions
void HPZEmerald_Update();
void HPZEmerald_EarlyUpdate();
void HPZEmerald_LateUpdate();
void HPZEmerald_Draw();
void HPZEmerald_Setup(void* subtype);
void HPZEmerald_StageLoad();
void HPZEmerald_GetAttributes();

#endif //!OBJ_HPZEMERALD_H
