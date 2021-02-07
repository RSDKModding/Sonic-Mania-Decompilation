#ifndef OBJ_ELECTROMAGNET_H
#define OBJ_ELECTROMAGNET_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectElectroMagnet : Object{

};

// Entity Class
struct EntityElectroMagnet : Entity {

};

// Object Struct
extern ObjectElectroMagnet *ElectroMagnet;

// Standard Entity Events
void ElectroMagnet_Update();
void ElectroMagnet_LateUpdate();
void ElectroMagnet_StaticUpdate();
void ElectroMagnet_Draw();
void ElectroMagnet_Create(void* data);
void ElectroMagnet_StageLoad();
void ElectroMagnet_EditorDraw();
void ElectroMagnet_EditorLoad();
void ElectroMagnet_Serialize();

// Extra Entity Functions


#endif //!OBJ_ELECTROMAGNET_H
