#ifndef OBJ_FERNPARALLAX_H
#define OBJ_FERNPARALLAX_H

#include "../../SonicMania.h"

//Object Class
class ObjectFernParallax : public Object {
public:

};

//Entity Class
class EntityFernParallax : public Entity {
public:

};

//Object Entity
ObjectFernParallax FernParallax;

//Entity Functions
void FernParallax_Update();
void FernParallax_EarlyUpdate();
void FernParallax_LateUpdate();
void FernParallax_Draw();
void FernParallax_Setup(void* subtype);
void FernParallax_StageLoad();
void FernParallax_GetAttributes();

#endif //!OBJ_FERNPARALLAX_H
