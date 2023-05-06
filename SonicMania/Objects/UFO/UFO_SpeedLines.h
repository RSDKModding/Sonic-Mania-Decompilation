#ifndef OBJ_UFO_SPEEDLINES_H
#define OBJ_UFO_SPEEDLINES_H

#include "Game.h"

#define UFO_SPEEDLINES_LINE_COUNT (32)

// Object Class
struct ObjectUFO_SpeedLines {
    RSDK_OBJECT
    int32 unused1;
    int32 unused2;
    int32 unused3;
};

// Entity Class
struct EntityUFO_SpeedLines {
    RSDK_ENTITY
    int32 lineX[UFO_SPEEDLINES_LINE_COUNT];
    int32 lineY[UFO_SPEEDLINES_LINE_COUNT];
    int32 lineZ[UFO_SPEEDLINES_LINE_COUNT];
    int32 lineAlpha[UFO_SPEEDLINES_LINE_COUNT];
    int32 unused1;
    int32 unused2;
};

// Object Struct
extern ObjectUFO_SpeedLines *UFO_SpeedLines;

// Standard Entity Events
void UFO_SpeedLines_Update(void);
void UFO_SpeedLines_LateUpdate(void);
void UFO_SpeedLines_StaticUpdate(void);
void UFO_SpeedLines_Draw(void);
void UFO_SpeedLines_Create(void *data);
void UFO_SpeedLines_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UFO_SpeedLines_EditorDraw(void);
void UFO_SpeedLines_EditorLoad(void);
#endif
void UFO_SpeedLines_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_UFO_SPEEDLINES_H
