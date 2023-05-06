#ifndef OBJ_TMZ2OUTRO_H
#define OBJ_TMZ2OUTRO_H

#include "Game.h"

// Object Class
struct ObjectTMZ2Outro {
    RSDK_OBJECT
    TABLE(color colors[7], { 0x400168, 0x700178, 0x900188, 0xB80188, 0xF00198, 0xF060B0, 0xF0A0C8 });
    uint16 sfxAlarm;
    uint16 unused;
    uint16 sfxGrab;
    uint16 sfxCarRev;
    uint16 sfxRumble;
    uint16 sfxDrop;
    uint16 sfxImpact;
    uint16 sfxRubyGet;
    bool32 finishedSaving;
    int32 playerID;
};

// Entity Class
struct EntityTMZ2Outro {
    RSDK_ENTITY
    EntityEscapeCar *escapeCar;
#if MANIA_USE_PLUS
    EntityCutsceneHBH *heavyKing;
#endif
};

// Object Struct
extern ObjectTMZ2Outro *TMZ2Outro;

// Standard Entity Events
void TMZ2Outro_Update(void);
void TMZ2Outro_LateUpdate(void);
void TMZ2Outro_StaticUpdate(void);
void TMZ2Outro_Draw(void);
void TMZ2Outro_Create(void *data);
void TMZ2Outro_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TMZ2Outro_EditorDraw(void);
void TMZ2Outro_EditorLoad(void);
#endif
void TMZ2Outro_Serialize(void);

// Extra Entity Functions
void TMZ2Outro_SetupCutscene(void);

bool32 TMZ2Outro_Cutscene_SetupOutro(EntityCutsceneSeq *host);
bool32 TMZ2Outro_Cutscene_WatchEggman(EntityCutsceneSeq *host);

void TMZ2Outro_PlayerStateInput_Escape(void);
bool32 TMZ2Outro_Cutscene_EggmanFall(EntityCutsceneSeq *host);
bool32 TMZ2Outro_Cutscene_StartAlert(EntityCutsceneSeq *host);
bool32 TMZ2Outro_Cutscene_TimeToEscape(EntityCutsceneSeq *host);
bool32 TMZ2Outro_Cutscene_HurryToCar(EntityCutsceneSeq *host);
bool32 TMZ2Outro_Cutscene_StartFadeOut(EntityCutsceneSeq *host);

#if MANIA_USE_PLUS
bool32 TMZ2Outro_Cutscene_StartRubyRampage(EntityCutsceneSeq *host);
bool32 TMZ2Outro_Cutscene_Panic(EntityCutsceneSeq *host);
bool32 TMZ2Outro_Cutscene_OuttaHere_BadEnd(EntityCutsceneSeq *host);
bool32 TMZ2Outro_Cutscene_OuttaHere(EntityCutsceneSeq *host);
bool32 TMZ2Outro_Cutscene_TeamEscape(EntityCutsceneSeq *host);
#endif

bool32 TMZ2Outro_Cutscene_FadeOut(EntityCutsceneSeq *host);
bool32 TMZ2Outro_Cutscene_FinishSequence(EntityCutsceneSeq *host);

#if MANIA_USE_PLUS
void TMZ2Outro_SaveFileCB(bool32 success);
#else
void TMZ2Outro_SaveFileCB(void);
#endif

#endif //! OBJ_TMZ2OUTRO_H
