#ifndef OBJ_LRZ2SETUP_H
#define OBJ_LRZ2SETUP_H

#include "Game.h"

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
void LRZ2Setup_Create(void *data);
void LRZ2Setup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void LRZ2Setup_EditorDraw(void);
void LRZ2Setup_EditorLoad(void);
#endif
void LRZ2Setup_Serialize(void);

// Extra Entity Functions
void LRZ2Setup_HandleStageReload(void);
void LRZ2Setup_Trigger_StartOutro(void);
EntityButton *LRZ2Setup_SetupTagLink(int32 tag, Entity *entity);
void LRZ2Setup_GetTileInfo(int32 x, int32 y, int32 moveOffsetX, int32 moveOffsetY, int32 cPlane, int32 *tile, uint8 *flags);

#endif //! OBJ_LRZ2SETUP_H
