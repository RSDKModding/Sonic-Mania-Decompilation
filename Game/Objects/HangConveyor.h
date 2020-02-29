#ifndef OBJ_HANGCONVEYOR_H
#define OBJ_HANGCONVEYOR_H

//Object Class
class HangConveyor : Object {

};

//Entity Class
class EntityHangConveyor : Entity {

};

//Entity Functions
void HangConveyor_Update();
void HangConveyor_EarlyUpdate();
void HangConveyor_LateUpdate();
void HangConveyor_Draw();
void HangConveyor_Setup(void* subtype);
void HangConveyor_StageLoad();
void HangConveyor_GetAttributes();

#endif //!OBJ_HANGCONVEYOR_H
