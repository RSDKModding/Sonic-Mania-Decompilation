#ifndef OBJ_TURNTABLE_H
#define OBJ_TURNTABLE_H

//Object Class
class Turntable : Object {

};

//Entity Class
class EntityTurntable : Entity {

};

//Entity Functions
void Turntable_Update();
void Turntable_EarlyUpdate();
void Turntable_LateUpdate();
void Turntable_Draw();
void Turntable_Setup(void* subtype);
void Turntable_StageLoad();
void Turntable_GetAttributes();

#endif //!OBJ_TURNTABLE_H
