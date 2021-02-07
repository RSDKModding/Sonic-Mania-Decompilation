#ifndef OBJ_LEDPANEL_H
#define OBJ_LEDPANEL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLEDPanel : Object{

};

// Entity Class
struct EntityLEDPanel : Entity {

};

// Object Struct
extern ObjectLEDPanel *LEDPanel;

// Standard Entity Events
void LEDPanel_Update();
void LEDPanel_LateUpdate();
void LEDPanel_StaticUpdate();
void LEDPanel_Draw();
void LEDPanel_Create(void* data);
void LEDPanel_StageLoad();
void LEDPanel_EditorDraw();
void LEDPanel_EditorLoad();
void LEDPanel_Serialize();

// Extra Entity Functions


#endif //!OBJ_LEDPANEL_H
