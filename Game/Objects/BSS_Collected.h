#ifndef OBJ_BSS_COLLECTED_H
#define OBJ_BSS_COLLECTED_H

#include "../../SonicMania.h"

//Object Class
class ObjectBSS_Collected : public Object {
public:

};

//Entity Class
class EntityBSS_Collected : public Entity {
public:

};

//Object Entity
ObjectBSS_Collected BSS_Collected;

//Entity Functions
void BSS_Collected_Update();
void BSS_Collected_EarlyUpdate();
void BSS_Collected_LateUpdate();
void BSS_Collected_Draw();
void BSS_Collected_Setup(void* subtype);
void BSS_Collected_StageLoad();
void BSS_Collected_GetAttributes();

#endif //!OBJ_BSS_COLLECTED_H
