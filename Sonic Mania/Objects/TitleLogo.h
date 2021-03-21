#ifndef OBJ_TITLELOGO_H
#define OBJ_TITLELOGO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort logoIndex;
#if RETRO_USE_PLUS
    ushort plusIndex;
    ushort sfx_Plus;
#endif
} ObjectTitleLogo;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int type;
    void (*state)();
    bool32 flag;
    int timer;
    int storeY;
    AnimationData data1;
    AnimationData data2;
    AnimationData data3;
} EntityTitleLogo;

// Object Struct
extern ObjectTitleLogo *TitleLogo;

// Standard Entity Events
void TitleLogo_Update();
void TitleLogo_LateUpdate();
void TitleLogo_StaticUpdate();
void TitleLogo_Draw();
void TitleLogo_Create(void* data);
void TitleLogo_StageLoad();
void TitleLogo_EditorDraw();
void TitleLogo_EditorLoad();
void TitleLogo_Serialize();

// Extra Entity Functions
void TitleLogo_Unknown1();
void TitleLogo_Unknown2();
void TitleLogo_Unknown3();
void TitleLogo_Unknown4();
void TitleLogo_Unknown5();
void TitleLogo_Unknown6();

#endif //!OBJ_TITLELOGO_H
