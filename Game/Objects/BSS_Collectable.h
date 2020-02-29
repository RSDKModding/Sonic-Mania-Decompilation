#ifndef OBJ_BSS_COLLECTABLE_H
#define OBJ_BSS_COLLECTABLE_H

#include "../../SonicMania.h"

//Object Class
class ObjectBSS_Collectable : public Object {
public:

};

//Entity Class
class EntityBSS_Collectable : public Entity {
public:

};

//Object Entity
ObjectBSS_Collectable BSS_Collectable;

//Entity Functions
void BSS_Collectable_Update();
void BSS_Collectable_EarlyUpdate();
void BSS_Collectable_LateUpdate();
void BSS_Collectable_Draw();
void BSS_Collectable_Setup(void* subtype);
void BSS_Collectable_StageLoad();
void BSS_Collectable_GetAttributes();

#endif //!OBJ_BSS_COLLECTABLE_H
