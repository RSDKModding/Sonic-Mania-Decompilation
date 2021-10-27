#ifndef OBJ_PBL_SETUP_H
#define OBJ_PBL_SETUP_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    int32 timer;
    int32 score;
    int32 score1UP;
    int32 rings;
    int32 timer2;
    int32 ringPan;
    int32 sectorID;
    int32 sectorCount;
    int32 timer3;
    uint8 tableLow;
    uint8 tableHigh;
    uint16 sfxContinue;
} ObjectPBL_Setup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 colour;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    int32 field_7C;
    int32 field_80;
    int32 field_84;
    int32 field_88;
    int32 field_8C;
    int32 field_90;
    int32 field_94;
    int32 field_98;
    int32 field_9C;
    int32 field_A0;
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
#if RETRO_INCLUDE_EDITOR
void PBL_Setup_EditorDraw(void);
void PBL_Setup_EditorLoad(void);
#endif
void PBL_Setup_Serialize(void);

// Extra Entity Functions
void PBL_Setup_TableLow_ScanlineCallback(ScanlineInfo *scanlines);
void PBL_Setup_TableHigh_ScanlineCallback(ScanlineInfo *scanlines);
void PBL_Setup_BG_ScanlineCallback(ScanlineInfo *scanlines);
void PBL_Setup_DrawLayer_Callback(void);

void PBL_Setup_Unknown5(void);
void PBL_Setup_GiveScore(int32 score);
void PBL_Setup_GiveLife(void);
void PBL_Setup_Unknown9(void);
void PBL_Setup_Unknown10(void);
void PBL_Setup_SaveAndChangeScene(void);

#endif

#endif //!OBJ_PBL_SETUP_H
