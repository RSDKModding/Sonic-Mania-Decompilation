#ifndef OBJ_PBL_SETUP_H
#define OBJ_PBL_SETUP_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    int timer;
    int score;
    int score1UP;
    int rings;
    int timer2;
    int ringPan;
    int sectorID;
    int sectorCount;
    int timer3;
    byte tableLow;
    byte tableHigh;
    ushort sfxContinue;
} ObjectPBL_Setup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
    int colour;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
} EntityPBL_Setup;

// Object Struct
extern ObjectPBL_Setup *PBL_Setup;

// Standard Entity Events
void PBL_Setup_Update(void);
void PBL_Setup_LateUpdate(void);
void PBL_Setup_StaticUpdate(void);
void PBL_Setup_Draw(void);
void PBL_Setup_Create(void* data);
void PBL_Setup_StageLoad(void);
void PBL_Setup_EditorDraw(void);
void PBL_Setup_EditorLoad(void);
void PBL_Setup_Serialize(void);

// Extra Entity Functions
void PBL_Setup_TableLow_ScanlineCallback(ScanlineInfo *scanlines);
void PBL_Setup_TableHigh_ScanlineCallback(ScanlineInfo *scanlines);
void PBL_Setup_BG_ScanlineCallback(ScanlineInfo *scanlines);
void PBL_Setup_DrawLayer_Callback(void);

void PBL_Setup_Unknown5(void);
void PBL_Setup_GiveScore(int score);
void PBL_Setup_GiveLife(void);
void PBL_Setup_Unknown9(void);
void PBL_Setup_Unknown10(void);
void PBL_Setup_SaveAndChangeScene(void);

#endif

#endif //!OBJ_PBL_SETUP_H
