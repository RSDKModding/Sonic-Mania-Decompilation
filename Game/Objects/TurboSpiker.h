#ifndef OBJ_TURBOSPIKER_H
#define OBJ_TURBOSPIKER_H

//Object Class
class TurboSpiker : Object {

};

//Entity Class
class EntityTurboSpiker : Entity {

};

//Entity Functions
void TurboSpiker_Update();
void TurboSpiker_EarlyUpdate();
void TurboSpiker_LateUpdate();
void TurboSpiker_Draw();
void TurboSpiker_Setup(void* subtype);
void TurboSpiker_StageLoad();
void TurboSpiker_GetAttributes();

#endif //!OBJ_TURBOSPIKER_H
