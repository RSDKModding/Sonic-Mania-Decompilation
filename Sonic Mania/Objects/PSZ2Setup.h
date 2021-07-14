#ifndef OBJ_PSZ2SETUP_H
#define OBJ_PSZ2SETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int aniTileDelays[8]; //= { 9, 7, 6, 7, 9, 7, 6, 7 };
    int aniTileDelay;
    int aniTileFrame;
    bool32 flag;
    short petalTimer;
    ushort aniTiles1;
    ushort aniTiles2;
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
