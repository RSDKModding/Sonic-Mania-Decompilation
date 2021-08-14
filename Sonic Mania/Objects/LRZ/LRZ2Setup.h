#ifndef OBJ_LRZ2SETUP_H
#define OBJ_LRZ2SETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int palTimer1;
    int palTimer2;
    int dstPal;
    int srcPal;
    int field_14;
    int field_18;
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
EntityButton *LRZ2Setup_HandleTagSetup(int tag, Entity *entityPtr);
void LRZ2Setup_GetTileInfo(int *tileInfo, int cPlane, int x, int y, int offsetX, int offsetY, byte *behaviour);

#endif //!OBJ_LRZ2SETUP_H
