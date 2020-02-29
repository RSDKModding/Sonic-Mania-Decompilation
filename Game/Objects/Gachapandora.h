#ifndef OBJ_GACHAPANDORA_H
#define OBJ_GACHAPANDORA_H

//Object Class
class Gachapandora : Object {

};

//Entity Class
class EntityGachapandora : Entity {

};

//Entity Functions
void Gachapandora_Update();
void Gachapandora_EarlyUpdate();
void Gachapandora_LateUpdate();
void Gachapandora_Draw();
void Gachapandora_Setup(void* subtype);
void Gachapandora_StageLoad();
void Gachapandora_GetAttributes();

#endif //!OBJ_GACHAPANDORA_H
