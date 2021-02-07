#ifndef OBJ_CONVEYORPLATFORM_H
#define OBJ_CONVEYORPLATFORM_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectConveyorPlatform : Object{

};

// Entity Class
struct EntityConveyorPlatform : Entity {

};

// Object Struct
extern ObjectConveyorPlatform *ConveyorPlatform;

// Standard Entity Events
void ConveyorPlatform_Update();
void ConveyorPlatform_LateUpdate();
void ConveyorPlatform_StaticUpdate();
void ConveyorPlatform_Draw();
void ConveyorPlatform_Create(void* data);
void ConveyorPlatform_StageLoad();
void ConveyorPlatform_EditorDraw();
void ConveyorPlatform_EditorLoad();
void ConveyorPlatform_Serialize();

// Extra Entity Functions


#endif //!OBJ_CONVEYORPLATFORM_H
