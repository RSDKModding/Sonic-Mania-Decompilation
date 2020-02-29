#ifndef OBJ_PETALPILE_H
#define OBJ_PETALPILE_H

//Object Class
class PetalPile : Object {

};

//Entity Class
class EntityPetalPile : Entity {

};

//Entity Functions
void PetalPile_Update();
void PetalPile_EarlyUpdate();
void PetalPile_LateUpdate();
void PetalPile_Draw();
void PetalPile_Setup(void* subtype);
void PetalPile_StageLoad();
void PetalPile_GetAttributes();

#endif //!OBJ_PETALPILE_H
