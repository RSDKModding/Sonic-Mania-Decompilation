#ifndef OBJ_TITLEBG_H
#define OBJ_TITLEBG_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int palTimer;
    int timer;
    int angle;
    ushort spriteIndex;
} ObjectTitleBG;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int type;
    int timer;
    AnimationData data;
} EntityTitleBG;

// Object Struct
extern ObjectTitleBG *TitleBG;

// Standard Entity Events
void TitleBG_Update();
void TitleBG_LateUpdate();
void TitleBG_StaticUpdate();
void TitleBG_Draw();
void TitleBG_Create(void* data);
void TitleBG_StageLoad();
void TitleBG_EditorDraw();
void TitleBG_EditorLoad();
void TitleBG_Serialize();

// Extra Entity Functions
void TitleBG_SetupFX();
void TitleBG_ScanlineCallbackA(ScanlineInfo *scanlines);
void TitleBG_ScanlineCallbackB(ScanlineInfo *scanlines);

#endif //!OBJ_TITLEBG_H
