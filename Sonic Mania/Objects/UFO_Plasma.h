#ifndef OBJ_UFO_PLASMA_H
#define OBJ_UFO_PLASMA_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    ScanlineInfo *scanlines;
    int deform[0x400];
    ushort spriteIndex;
    int field_1010;
} ObjectUFO_Plasma;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUFO_Plasma;

// Object Struct
extern ObjectUFO_Plasma *UFO_Plasma;

// Standard Entity Events
void UFO_Plasma_Update(void);
void UFO_Plasma_LateUpdate(void);
void UFO_Plasma_StaticUpdate(void);
void UFO_Plasma_Draw(void);
void UFO_Plasma_Create(void* data);
void UFO_Plasma_StageLoad(void);
void UFO_Plasma_EditorDraw(void);
void UFO_Plasma_EditorLoad(void);
void UFO_Plasma_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UFO_PLASMA_H
