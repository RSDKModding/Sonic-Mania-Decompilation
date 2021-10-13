#ifndef OBJ_UFO_WATER_H
#define OBJ_UFO_WATER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 timer;
    uint16 spriteIndex;
} ObjectUFO_Water;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUFO_Water;

// Object Struct
extern ObjectUFO_Water *UFO_Water;

// Standard Entity Events
void UFO_Water_Update(void);
void UFO_Water_LateUpdate(void);
void UFO_Water_StaticUpdate(void);
void UFO_Water_Draw(void);
void UFO_Water_Create(void* data);
void UFO_Water_StageLoad(void);
void UFO_Water_EditorDraw(void);
void UFO_Water_EditorLoad(void);
void UFO_Water_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UFO_WATER_H
