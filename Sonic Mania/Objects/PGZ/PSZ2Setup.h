#ifndef OBJ_PSZ2SETUP_H
#define OBJ_PSZ2SETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 aniTileDelays[8], { 9, 7, 6, 7, 9, 7, 6, 7 });
    int32 aniTileDelay;
    int32 aniTileFrame;
    bool32 flag;
    int16 petalTimer;
    uint16 aniTiles1;
    uint16 aniTiles2;
} ObjectPSZ2Setup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPSZ2Setup;

// Object Struct
extern ObjectPSZ2Setup *PSZ2Setup;

// Standard Entity Events
void PSZ2Setup_Update(void);
void PSZ2Setup_LateUpdate(void);
void PSZ2Setup_StaticUpdate(void);
void PSZ2Setup_Draw(void);
void PSZ2Setup_Create(void* data);
void PSZ2Setup_StageLoad(void);
void PSZ2Setup_EditorDraw(void);
void PSZ2Setup_EditorLoad(void);
void PSZ2Setup_Serialize(void);

// Extra Entity Functions
void PSZ2Setup_TriggerCB2(void);
void PSZ2Setup_TriggerCB1(void);
void PSZ2Setup_ActTransitionLoad(void);
void PSZ2Setup_StageFinishCB(void);

#endif //!OBJ_PSZ2SETUP_H
