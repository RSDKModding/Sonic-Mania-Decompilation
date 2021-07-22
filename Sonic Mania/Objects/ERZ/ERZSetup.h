#ifndef OBJ_ERZSETUP_H
#define OBJ_ERZSETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int aniTileTimer;
    int timer;
    ushort skySheet;
} ObjectERZSetup;

// Entity Class
typedef struct {
    RSDK_ENTITY
} EntityERZSetup;

// Object Struct
extern ObjectERZSetup *ERZSetup;

// Standard Entity Events
void ERZSetup_Update(void);
void ERZSetup_LateUpdate(void);
void ERZSetup_StaticUpdate(void);
void ERZSetup_Draw(void);
void ERZSetup_Create(void* data);
void ERZSetup_StageLoad(void);
void ERZSetup_EditorDraw(void);
void ERZSetup_EditorLoad(void);
void ERZSetup_Serialize(void);

// Extra Entity Functions
void ERZSetup_ScanlineCallback(ScanlineInfo *scanlines);

#endif //!OBJ_ERZSETUP_H
