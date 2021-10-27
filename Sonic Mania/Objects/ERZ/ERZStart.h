#ifndef OBJ_ERZSTART_H
#define OBJ_ERZSTART_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 timer;
    int32 field_8;
    int32 superDashCooldown;
    int32 field_10;
    uint16 sfxClack;
    uint16 sfxFlyIn;
    uint16 sfxHullClose;
    uint16 sfxRumble;
    Entity *fxRuby;
    Entity *ruby;
    EntityChaosEmerald *emeralds[7];
    Entity *king;
    Entity *eggman;
} ObjectERZStart;

// Entity Class
typedef struct {
    MANIA_CUTSCENE_BASE
    int32 timer;
} EntityERZStart;

// Object Struct
extern ObjectERZStart *ERZStart;

// Standard Entity Events
void ERZStart_Update(void);
void ERZStart_LateUpdate(void);
void ERZStart_StaticUpdate(void);
void ERZStart_Draw(void);
void ERZStart_Create(void* data);
void ERZStart_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void ERZStart_EditorDraw(void);
void ERZStart_EditorLoad(void);
#endif
void ERZStart_Serialize(void);

// Extra Entity Functions
void ERZStart_SetupObjects(void);
void ERZStart_HandlePlayerHover(EntityCutsceneSeq *seq, void *p, int posY);

bool32 ERZStart_CutsceneState_Unknown1(EntityCutsceneSeq *host);
bool32 ERZStart_CutsceneState_Unknown2(EntityCutsceneSeq *host);
bool32 ERZStart_CutsceneState_Unknown3(EntityCutsceneSeq *host);
bool32 ERZStart_CutsceneState_Unknown4(EntityCutsceneSeq *host);
bool32 ERZStart_CutsceneState_Unknown5(EntityCutsceneSeq *host);
bool32 ERZStart_CutsceneState_Unknown6(EntityCutsceneSeq *host);
bool32 ERZStart_CutsceneState_Unknown7(EntityCutsceneSeq *host);
bool32 ERZStart_CutsceneState_Unknown8(EntityCutsceneSeq *host);
bool32 ERZStart_CutsceneState_Unknown9(EntityCutsceneSeq *host);
bool32 ERZStart_CutsceneState_Unknown10(EntityCutsceneSeq *host);
bool32 ERZStart_CutsceneState_Unknown11(EntityCutsceneSeq *host);
bool32 ERZStart_CutsceneState_PlayerTransform(EntityCutsceneSeq *host);
bool32 ERZStart_CutsceneState_StartFight(EntityCutsceneSeq *host);
bool32 ERZStart_CutsceneState_Fight(EntityCutsceneSeq *host);

void ERZStart_RubyHover(void);
void ERZStart_RubyMove(void);

void ERZStart_Player_HandleSuperDash(void *p);
void ERZStart_State_PlayerSuperFly(void);
void ERZStart_Player_StartSuperFly(void);
void ERZStart_State_PlayerRebound(void);

#endif //!OBJ_ERZSTART_H
