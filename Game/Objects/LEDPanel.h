#ifndef OBJ_LEDPANEL_H
#define OBJ_LEDPANEL_H

//Object Class
class LEDPanel : Object {

};

//Entity Class
class EntityLEDPanel : Entity {

};

//Entity Functions
void LEDPanel_Update();
void LEDPanel_EarlyUpdate();
void LEDPanel_LateUpdate();
void LEDPanel_Draw();
void LEDPanel_Setup(void* subtype);
void LEDPanel_StageLoad();
void LEDPanel_GetAttributes();

#endif //!OBJ_LEDPANEL_H
