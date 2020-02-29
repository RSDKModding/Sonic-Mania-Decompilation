#ifndef OBJ_JELLYGNITE_H
#define OBJ_JELLYGNITE_H

#include "../../SonicMania.h"

//Object Class
class ObjectJellygnite : public Object {
public:

};

//Entity Class
class EntityJellygnite : public Entity {
public:

};

//Object Entity
ObjectJellygnite Jellygnite;

//Entity Functions
void Jellygnite_Update();
void Jellygnite_EarlyUpdate();
void Jellygnite_LateUpdate();
void Jellygnite_Draw();
void Jellygnite_Setup(void* subtype);
void Jellygnite_StageLoad();
void Jellygnite_GetAttributes();

#endif //!OBJ_JELLYGNITE_H
