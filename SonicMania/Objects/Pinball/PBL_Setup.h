#ifndef OBJ_PBL_SETUP_H
#define OBJ_PBL_SETUP_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectPBL_Setup {
    RSDK_OBJECT
    int32 unused1;
    int32 timer;
    int32 score;
    int32 score1UP;
    int32 rings;
    int32 ringFrame;
    int32 ringPan;
    int32 sectorID;
    int32 sectorCount;
    int32 scanlineTimer;
    uint8 tableLow;
    uint8 tableHigh;
    uint16 sfxContinue;
};

// Entity Class
struct EntityPBL_Setup {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 color;
    Matrix unusedMatrix1;
};

// Object Struct
extern ObjectPBL_Setup *PBL_Setup;

// Standard Entity Events
void PBL_Setup_Update(void);
void PBL_Setup_LateUpdate(void);
void PBL_Setup_StaticUpdate(void);
void PBL_Setup_Draw(void);
void PBL_Setup_Create(void *data);
void PBL_Setup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PBL_Setup_EditorDraw(void);
void PBL_Setup_EditorLoad(void);
#endif
void PBL_Setup_Serialize(void);

// Extra Entity Functions
void PBL_Setup_Scanline_TableLow(ScanlineInfo *scanlines);
void PBL_Setup_Scanline_TableHigh(ScanlineInfo *scanlines);
void PBL_Setup_Scanline_PinballBG(ScanlineInfo *scanlines);
void PBL_Setup_DrawHook_PrepareDrawingFX(void);

void PBL_Setup_ExitPinball(void);
void PBL_Setup_GiveScore(int32 score);
void PBL_Setup_GiveLife(void);
void PBL_Setup_State_FadeIn(void);
void PBL_Setup_State_ManageStageExit(void);
void PBL_Setup_SaveAndChangeScene(void);

#endif

#endif //! OBJ_PBL_SETUP_H
