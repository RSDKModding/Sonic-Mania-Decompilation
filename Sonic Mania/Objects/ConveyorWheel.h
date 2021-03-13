#ifndef OBJ_CONVEYORWHEEL_H
#define OBJ_CONVEYORWHEEL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectConveyorWheel;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityConveyorWheel;

// Object Struct
extern ObjectConveyorWheel *ConveyorWheel;

// Standard Entity Events
void ConveyorWheel_Update();
void ConveyorWheel_LateUpdate();
void ConveyorWheel_StaticUpdate();
void ConveyorWheel_Draw();
void ConveyorWheel_Create(void* data);
void ConveyorWheel_StageLoad();
void ConveyorWheel_EditorDraw();
void ConveyorWheel_EditorLoad();
void ConveyorWheel_Serialize();

// Extra Entity Functions


#endif //!OBJ_CONVEYORWHEEL_H
