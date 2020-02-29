#ifndef OBJ_BSS_SETUP_H
#define OBJ_BSS_SETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectBSS_Setup : public Object {
public:

};

//Entity Class
class EntityBSS_Setup : public Entity {
public:

};

//Object Entity
ObjectBSS_Setup BSS_Setup;

//Entity Functions
void BSS_Setup_Update();
void BSS_Setup_EarlyUpdate();
void BSS_Setup_LateUpdate();
void BSS_Setup_Draw();
void BSS_Setup_Setup(void* subtype);
void BSS_Setup_StageLoad();
void BSS_Setup_GetAttributes();

#endif //!OBJ_BSS_SETUP_H
