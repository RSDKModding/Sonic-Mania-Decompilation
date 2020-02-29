#ifndef OBJ_GACHAPANDORA_H
#define OBJ_GACHAPANDORA_H

#include "../../SonicMania.h"

//Object Class
class ObjectGachapandora : public Object {
public:

};

//Entity Class
class EntityGachapandora : public Entity {
public:

};

//Object Entity
ObjectGachapandora Gachapandora;

//Entity Functions
void Gachapandora_Update();
void Gachapandora_EarlyUpdate();
void Gachapandora_LateUpdate();
void Gachapandora_Draw();
void Gachapandora_Setup(void* subtype);
void Gachapandora_StageLoad();
void Gachapandora_GetAttributes();

#endif //!OBJ_GACHAPANDORA_H
