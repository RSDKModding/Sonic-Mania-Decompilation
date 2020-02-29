#ifndef OBJ_LEDPANEL_H
#define OBJ_LEDPANEL_H

#include "../../SonicMania.h"

//Object Class
class ObjectLEDPanel : public Object {
public:

};

//Entity Class
class EntityLEDPanel : public Entity {
public:

};

//Object Entity
ObjectLEDPanel LEDPanel;

//Entity Functions
void LEDPanel_Update();
void LEDPanel_EarlyUpdate();
void LEDPanel_LateUpdate();
void LEDPanel_Draw();
void LEDPanel_Setup(void* subtype);
void LEDPanel_StageLoad();
void LEDPanel_GetAttributes();

#endif //!OBJ_LEDPANEL_H
