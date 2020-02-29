#ifndef OBJ_ESCAPECAR_H
#define OBJ_ESCAPECAR_H

//Object Class
class EscapeCar : Object {

};

//Entity Class
class EntityEscapeCar : Entity {

};

//Entity Functions
void EscapeCar_Update();
void EscapeCar_EarlyUpdate();
void EscapeCar_LateUpdate();
void EscapeCar_Draw();
void EscapeCar_Setup(void* subtype);
void EscapeCar_StageLoad();
void EscapeCar_GetAttributes();

#endif //!OBJ_ESCAPECAR_H
