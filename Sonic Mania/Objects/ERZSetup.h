#ifndef OBJ_ERZSETUP_H
#define OBJ_ERZSETUP_H

#include "../SonicMania.h"

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
void ERZSetup_Update();
void ERZSetup_LateUpdate();
void ERZSetup_StaticUpdate();
void ERZSetup_Draw();
void ERZSetup_Create(void* data);
void ERZSetup_StageLoad();
void ERZSetup_EditorDraw();
void ERZSetup_EditorLoad();
void ERZSetup_Serialize();

// Extra Entity Functions
void ERZSetup_ScanlineCallback(ScanlineInfo *scanlines);

#endif //!OBJ_ERZSETUP_H
