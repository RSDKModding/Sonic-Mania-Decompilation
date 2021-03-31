#ifndef OBJ_UFO_SPEEDLINES_H
#define OBJ_UFO_SPEEDLINES_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    int field_8;
    int field_C;
} ObjectUFO_SpeedLines;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int lineX[32];
    int lineY[32];
    int lineZ[32];
    int lineAlpha[32];
    int field_258;
    int field_25C;
} EntityUFO_SpeedLines;

// Object Struct
extern ObjectUFO_SpeedLines *UFO_SpeedLines;

// Standard Entity Events
void UFO_SpeedLines_Update();
void UFO_SpeedLines_LateUpdate();
void UFO_SpeedLines_StaticUpdate();
void UFO_SpeedLines_Draw();
void UFO_SpeedLines_Create(void* data);
void UFO_SpeedLines_StageLoad();
void UFO_SpeedLines_EditorDraw();
void UFO_SpeedLines_EditorLoad();
void UFO_SpeedLines_Serialize();

// Extra Entity Functions


#endif //!OBJ_UFO_SPEEDLINES_H
