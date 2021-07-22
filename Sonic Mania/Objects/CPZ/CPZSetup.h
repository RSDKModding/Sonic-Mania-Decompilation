#ifndef OBJ_CPZSETUP_H
#define OBJ_CPZSETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniTiles;
    TileLayer* layerPtr;
    int deformation[64]; //= { 1, 2, 1, 3, 1, 2, 2, 1, 2, 3, 1, 2, 1, 2, 0, 0, 2, 0, 3, 2, 2, 3, 2, 2, 1, 3, 0, 0, 1, 0, 1, 3, 1, 2, 1, 3, 1, 2, 2, 1, 2, 3, 1, 2, 1, 2, 0, 0, 2, 0, 3, 2, 2, 3, 2, 2, 1, 3, 0, 0, 1, 0, 1, 3 };
    int palAnimFrameA;
    int palAnimFrameB;
    int palAnimFrameC;
    int palAnimFrameD;
    int palAnimFrameE;
    ushort aniTileFrame;
    Entity* outro;
} ObjectCPZSetup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCPZSetup;

// Object Struct
extern ObjectCPZSetup *CPZSetup;

// Standard Entity Events
void CPZSetup_Update(void);
void CPZSetup_LateUpdate(void);
void CPZSetup_StaticUpdate(void);
void CPZSetup_Draw(void);
void CPZSetup_Create(void* data);
void CPZSetup_StageLoad(void);
void CPZSetup_EditorDraw(void);
void CPZSetup_EditorLoad(void);
void CPZSetup_Serialize(void);

// Extra Entity Functions
void CPZSetup_BGSwitchCB1(void);
void CPZSetup_BGSwitchCB2(void);
void CPZSetup_StageFinishCBAct1(void);
void CPZSetup_StageFinishCBAct2(void);

#endif //!OBJ_CPZSETUP_H
