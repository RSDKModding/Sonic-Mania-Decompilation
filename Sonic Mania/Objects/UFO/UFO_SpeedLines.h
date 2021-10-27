#ifndef OBJ_UFO_SPEEDLINES_H
#define OBJ_UFO_SPEEDLINES_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    int32 field_8;
    int32 field_C;
} ObjectUFO_SpeedLines;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 lineX[32];
    int32 lineY[32];
    int32 lineZ[32];
    int32 lineAlpha[32];
    int32 field_258;
    int32 field_25C;
} EntityUFO_SpeedLines;

// Object Struct
extern ObjectUFO_SpeedLines *UFO_SpeedLines;

// Standard Entity Events
void UFO_SpeedLines_Update(void);
void UFO_SpeedLines_LateUpdate(void);
void UFO_SpeedLines_StaticUpdate(void);
void UFO_SpeedLines_Draw(void);
void UFO_SpeedLines_Create(void* data);
void UFO_SpeedLines_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UFO_SpeedLines_EditorDraw(void);
void UFO_SpeedLines_EditorLoad(void);
#endif
void UFO_SpeedLines_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UFO_SPEEDLINES_H
