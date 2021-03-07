#ifndef OBJ_CONVEYORBELT_H
#define OBJ_CONVEYORBELT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectConveyorBelt : Object {

};

// Entity Class
struct EntityConveyorBelt : Entity {

};

// Object Struct
extern ObjectConveyorBelt *ConveyorBelt;

// Standard Entity Events
void ConveyorBelt_Update();
void ConveyorBelt_LateUpdate();
void ConveyorBelt_StaticUpdate();
void ConveyorBelt_Draw();
void ConveyorBelt_Create(void* data);
void ConveyorBelt_StageLoad();
void ConveyorBelt_EditorDraw();
void ConveyorBelt_EditorLoad();
void ConveyorBelt_Serialize();

// Extra Entity Functions


#endif //!OBJ_CONVEYORBELT_H
