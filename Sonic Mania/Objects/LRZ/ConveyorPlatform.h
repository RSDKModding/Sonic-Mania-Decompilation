#ifndef OBJ_CONVEYORPLATFORM_H
#define OBJ_CONVEYORPLATFORM_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[24]; //= { 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1 };
    int value2[24]; //= { 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1 };
} ObjectConveyorPlatform;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityConveyorPlatform;

// Object Struct
extern ObjectConveyorPlatform *ConveyorPlatform;

// Standard Entity Events
void ConveyorPlatform_Update(void);
void ConveyorPlatform_LateUpdate(void);
void ConveyorPlatform_StaticUpdate(void);
void ConveyorPlatform_Draw(void);
void ConveyorPlatform_Create(void* data);
void ConveyorPlatform_StageLoad(void);
void ConveyorPlatform_EditorDraw(void);
void ConveyorPlatform_EditorLoad(void);
void ConveyorPlatform_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CONVEYORPLATFORM_H
