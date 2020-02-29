#ifndef OBJ_BOUNCEPLANT_H
#define OBJ_BOUNCEPLANT_H

//Object Class
class BouncePlant : Object {

};

//Entity Class
class EntityBouncePlant : Entity {

};

//Entity Functions
void BouncePlant_Update();
void BouncePlant_EarlyUpdate();
void BouncePlant_LateUpdate();
void BouncePlant_Draw();
void BouncePlant_Setup(void* subtype);
void BouncePlant_StageLoad();
void BouncePlant_GetAttributes();

#endif //!OBJ_BOUNCEPLANT_H
