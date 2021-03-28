#ifndef OBJ_THANKSSETUP_H
#define OBJ_THANKSSETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort sfx_Sega;
} ObjectThanksSetup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void(*state)();
    void(*stateDraw)();
    Entity *picture;
    int timer;
    int offset;
} EntityThanksSetup;

// Object Struct
extern ObjectThanksSetup *ThanksSetup;

// Standard Entity Events
void ThanksSetup_Update();
void ThanksSetup_LateUpdate();
void ThanksSetup_StaticUpdate();
void ThanksSetup_Draw();
void ThanksSetup_Create(void* data);
void ThanksSetup_StageLoad();
void ThanksSetup_EditorDraw();
void ThanksSetup_EditorLoad();
void ThanksSetup_Serialize();

// Extra Entity Functions
void ThanksSetup_Unknown1();
void ThanksSetup_Unknown2();
void ThanksSetup_Unknown3();
void ThanksSetup_Unknown4();
void ThanksSetup_Unknown5();
void ThanksSetup_Unknown6();

#endif //!OBJ_THANKSSETUP_H
