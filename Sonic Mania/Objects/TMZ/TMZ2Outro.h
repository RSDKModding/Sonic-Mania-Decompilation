#ifndef OBJ_TMZ2OUTRO_H
#define OBJ_TMZ2OUTRO_H

#include "SonicMania.h"

// Object Class
struct ObjectTMZ2Outro {
    RSDK_OBJECT
    TABLE(colour colours[7], { 0x400168, 0x700178, 0x900188, 0xB80188, 0xF00198, 0xF060B0, 0xF0A0C8 });
    uint16 sfxAlarm;
    uint16 field_22;
    uint16 sfxGrab;
    uint16 sfxCarRev;
    uint16 sfxRumble;
    uint16 sfxDrop;
    uint16 sfxImpact;
    uint16 sfxRubyGet;
    bool32 flag1;
    int32 playerID;
};

// Entity Class
struct EntityTMZ2Outro {
    RSDK_ENTITY
    EntityEscapeCar *escapeCar;
#if RETRO_USE_PLUS
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
void TMZ2Outro_Create(void* data);
void TMZ2Outro_StageLoad(void);
void TMZ2Outro_EditorDraw(void);
void TMZ2Outro_EditorLoad(void);
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

#if RETRO_USE_PLUS
bool32 TMZ2Outro_Cutscene_StartRubyRampage(EntityCutsceneSeq *host);
bool32 TMZ2Outro_Cutscene_Panic(EntityCutsceneSeq *host);
bool32 TMZ2Outro_Cutscene_OuttaHere_BadEnd(EntityCutsceneSeq *host);
bool32 TMZ2Outro_Cutscene_OuttaHere(EntityCutsceneSeq *host);
bool32 TMZ2Outro_Cutscene_TeamEscape(EntityCutsceneSeq *host);
#endif

bool32 TMZ2Outro_Cutscene_FadeOut(EntityCutsceneSeq *host);
bool32 TMZ2Outro_Cutscene_FinishSequence(EntityCutsceneSeq *host);

void TMZ2Outro_SaveFileCB(bool32 success);

#endif //!OBJ_TMZ2OUTRO_H
