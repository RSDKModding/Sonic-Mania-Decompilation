#ifndef OBJ_MEGAOCTUS_H
#define OBJ_MEGAOCTUS_H

#include "../../SonicMania.h"

//Object Class
class ObjectMegaOctus : public Object {
public:

};

//Entity Class
class EntityMegaOctus : public Entity {
public:

};

//Object Entity
ObjectMegaOctus MegaOctus;

//Entity Functions
void MegaOctus_Update();
void MegaOctus_EarlyUpdate();
void MegaOctus_LateUpdate();
void MegaOctus_Draw();
void MegaOctus_Setup(void* subtype);
void MegaOctus_StageLoad();
void MegaOctus_GetAttributes();

#endif //!OBJ_MEGAOCTUS_H
