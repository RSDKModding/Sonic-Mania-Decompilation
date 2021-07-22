#ifndef OBJ_TITLEBG_H
#define OBJ_TITLEBG_H

#include "SonicMania.h"

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
    Animator animator;
} EntityTitleBG;

// Object Struct
extern ObjectTitleBG *TitleBG;

// Standard Entity Events
void TitleBG_Update(void);
void TitleBG_LateUpdate(void);
void TitleBG_StaticUpdate(void);
void TitleBG_Draw(void);
void TitleBG_Create(void* data);
void TitleBG_StageLoad(void);
void TitleBG_EditorDraw(void);
void TitleBG_EditorLoad(void);
void TitleBG_Serialize(void);

// Extra Entity Functions
void TitleBG_SetupFX(void);
void TitleBG_ScanlineCallbackA(ScanlineInfo *scanlines);
void TitleBG_ScanlineCallbackB(ScanlineInfo *scanlines);

#endif //!OBJ_TITLEBG_H
