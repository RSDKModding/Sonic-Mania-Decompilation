#ifndef OBJ_PSZEGGMAN_H
#define OBJ_PSZEGGMAN_H

//Object Class
class PSZEggman : Object {

};

//Entity Class
class EntityPSZEggman : Entity {

};

//Entity Functions
void PSZEggman_Update();
void PSZEggman_EarlyUpdate();
void PSZEggman_LateUpdate();
void PSZEggman_Draw();
void PSZEggman_Setup(void* subtype);
void PSZEggman_StageLoad();
void PSZEggman_GetAttributes();

#endif //!OBJ_PSZEGGMAN_H
