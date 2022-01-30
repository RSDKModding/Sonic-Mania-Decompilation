#ifndef OBJ_LRZ2SETUP_H
#define OBJ_LRZ2SETUP_H

#include "SonicMania.h"

typedef enum { GENERICTRIGGER_LRZ2_OUTRO } GenericTriggerTypesLRZ2;

typedef enum { LRZ2_TFLAGS_NORMAL, LRZ2_TFLAGS_LAVA, LRZ2_TFLAGS_CONVEYOR_L, LRZ2_TFLAGS_CONVEYOR_R } TileFlagsLRZ2;

// Object Class
struct ObjectLRZ2Setup {
    RSDK_OBJECT
    int32 lavaPalTimer;
    int32 conveyorPalTimer;
    int32 conveyorDstPal;
    int32 conveyorSrcPal;
    bool32 conveyorOff;
    uint8 conveyorDir;
};

// Entity Class
struct EntityLRZ2Setup {
	RSDK_ENTITY
};

// Object Struct
extern ObjectLRZ2Setup *LRZ2Setup;

// Standard Entity Events
void LRZ2Setup_Update(void);
void LRZ2Setup_LateUpdate(void);
void LRZ2Setup_StaticUpdate(void);
void LRZ2Setup_Draw(void);
void LRZ2Setup_Create(void* data);
void LRZ2Setup_StageLoad(void);
void LRZ2Setup_EditorDraw(void);
void LRZ2Setup_EditorLoad(void);
void LRZ2Setup_Serialize(void);

// Extra Entity Functions
void LRZ2Setup_HandleStageReload(void);
void LRZ2Setup_GenericTrigger_CB(void);
EntityButton *LRZ2Setup_HandleTagSetup(int32 tag, Entity *entityPtr);
void LRZ2Setup_GetTileInfo(int32 *tileInfo, int32 cPlane, int32 x, int32 y, int32 offsetX, int32 offsetY, uint8 *behaviour);

#endif //!OBJ_LRZ2SETUP_H
