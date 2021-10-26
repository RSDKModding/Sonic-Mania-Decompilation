#ifndef OBJ_ROTATINGSTAIR_H
#define OBJ_ROTATINGSTAIR_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
    uint8 flag;
} ObjectRotatingStair;

// Entity Class
typedef struct {
	MANIA_PLATFORM_BASE
	uint16 oscOff;
	int32 duration;
	int32 interval;
} EntityRotatingStair;

// Object Struct
extern ObjectRotatingStair *RotatingStair;

// Standard Entity Events
void RotatingStair_Update(void);
void RotatingStair_LateUpdate(void);
void RotatingStair_StaticUpdate(void);
void RotatingStair_Draw(void);
void RotatingStair_Create(void* data);
void RotatingStair_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void RotatingStair_EditorDraw(void);
void RotatingStair_EditorLoad(void);
#endif
void RotatingStair_Serialize(void);

// Extra Entity Functions
void RotatingStair_Unknown1(void);
void RotatingStair_Unknown2(void);

#endif //!OBJ_ROTATINGSTAIR_H
