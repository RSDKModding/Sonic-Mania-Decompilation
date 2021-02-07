#ifndef OBJ_BGSWITCH_H
#define OBJ_BGSWITCH_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBGSwitch : Object{

};

// Entity Class
struct EntityBGSwitch : Entity {

};

// Object Struct
extern ObjectBGSwitch *BGSwitch;

// Standard Entity Events
void BGSwitch_Update();
void BGSwitch_LateUpdate();
void BGSwitch_StaticUpdate();
void BGSwitch_Draw();
void BGSwitch_Create(void* data);
void BGSwitch_StageLoad();
void BGSwitch_EditorDraw();
void BGSwitch_EditorLoad();
void BGSwitch_Serialize();

// Extra Entity Functions


#endif //!OBJ_BGSWITCH_H
