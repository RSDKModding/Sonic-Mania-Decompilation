#ifndef OBJ_COLLAPSINGSAND_H
#define OBJ_COLLAPSINGSAND_H

#include "../../SonicMania.h"

//Object Class
class ObjectCollapsingSand : public Object {
public:

};

//Entity Class
class EntityCollapsingSand : public Entity {
public:

};

//Object Entity
ObjectCollapsingSand CollapsingSand;

//Entity Functions
void CollapsingSand_Update();
void CollapsingSand_EarlyUpdate();
void CollapsingSand_LateUpdate();
void CollapsingSand_Draw();
void CollapsingSand_Setup(void* subtype);
void CollapsingSand_StageLoad();
void CollapsingSand_GetAttributes();

#endif //!OBJ_COLLAPSINGSAND_H
