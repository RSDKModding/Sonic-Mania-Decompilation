#ifndef OBJ_SPZ1SETUP_H
#define OBJ_SPZ1SETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int palRotateTimerA;
    int palRotateTimerB;
    int angle;
    int aniTilesFrameC;
    int timerA;
    int timerB;
    int aniTilesFrameB;
    ushort aniTiles;
    TileLayer *bgLayer;
    TileLayer *bgLayer2;
} ObjectSPZ1Setup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySPZ1Setup;

// Object Struct
extern ObjectSPZ1Setup *SPZ1Setup;

// Standard Entity Events
void SPZ1Setup_Update(void);
void SPZ1Setup_LateUpdate(void);
void SPZ1Setup_StaticUpdate(void);
void SPZ1Setup_Draw(void);
void SPZ1Setup_Create(void* data);
void SPZ1Setup_StageLoad(void);
void SPZ1Setup_EditorDraw(void);
void SPZ1Setup_EditorLoad(void);
void SPZ1Setup_Serialize(void);

// Extra Entity Functions
void SPZ1Setup_SetupActTransition(void);

#endif //!OBJ_SPZ1SETUP_H
