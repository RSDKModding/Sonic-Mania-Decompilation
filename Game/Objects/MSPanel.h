#ifndef OBJ_MSPANEL_H
#define OBJ_MSPANEL_H

#include "../../SonicMania.h"

//Object Class
class ObjectMSPanel : public Object {
public:

};

//Entity Class
class EntityMSPanel : public Entity {
public:

};

//Object Entity
ObjectMSPanel MSPanel;

//Entity Functions
void MSPanel_Update();
void MSPanel_EarlyUpdate();
void MSPanel_LateUpdate();
void MSPanel_Draw();
void MSPanel_Setup(void* subtype);
void MSPanel_StageLoad();
void MSPanel_GetAttributes();

#endif //!OBJ_MSPANEL_H
