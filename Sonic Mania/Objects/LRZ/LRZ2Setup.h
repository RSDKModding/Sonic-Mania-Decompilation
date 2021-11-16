#ifndef OBJ_LRZ2SETUP_H
#define OBJ_LRZ2SETUP_H

#include "SonicMania.h"

typedef enum {
    GENERICTRIGGER_LRZ2_OUTRO
}GenericTriggerTypesLRZ2;

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 palTimer1;
    int32 palTimer2;
    int32 dstPal;
    int32 srcPal;
    bool32 conveyorOff;
    uint8 conveyorDir;
} ObjectLRZ2Setup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLRZ2Setup;

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
