#ifndef OBJ_HONKYTONK_H
#define OBJ_HONKYTONK_H

//Object Class
class Honkytonk : Object {

};

//Entity Class
class EntityHonkytonk : Entity {

};

//Entity Functions
void Honkytonk_Update();
void Honkytonk_EarlyUpdate();
void Honkytonk_LateUpdate();
void Honkytonk_Draw();
void Honkytonk_Setup(void* subtype);
void Honkytonk_StageLoad();
void Honkytonk_GetAttributes();

#endif //!OBJ_HONKYTONK_H
