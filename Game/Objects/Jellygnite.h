#ifndef OBJ_JELLYGNITE_H
#define OBJ_JELLYGNITE_H

//Object Class
class Jellygnite : Object {

};

//Entity Class
class EntityJellygnite : Entity {

};

//Entity Functions
void Jellygnite_Update();
void Jellygnite_EarlyUpdate();
void Jellygnite_LateUpdate();
void Jellygnite_Draw();
void Jellygnite_Setup(void* subtype);
void Jellygnite_StageLoad();
void Jellygnite_GetAttributes();

#endif //!OBJ_JELLYGNITE_H
