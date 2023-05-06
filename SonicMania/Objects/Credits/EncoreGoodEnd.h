#ifndef OBJ_ENCOREGOODEND_H
#define OBJ_ENCOREGOODEND_H

#include "Game.h"

#if MANIA_USE_PLUS

typedef enum {
    E_END_DECOR_STOOL,
    E_END_DECOR_ICECREAM_PINK,
    E_END_DECOR_ICECREAM_GREEN,
    E_END_DECOR_ICECREAM_CONE,
    E_END_DECOR_KING,
    E_END_DECOR_RAYSPIN,
    E_END_DECOR_RAYSHOCKED,
    E_END_DECOR_SONICLAYDOWN,
    E_END_DECOR_SONICSHOCKED,
    E_END_DECOR_KNUXBREATHE,
    E_END_DECOR_KNUXCLINK,
    E_END_DECOR_KNUXCLINKG,
    E_END_DECOR_KNUXSHOCKED,
    E_END_DECOR_KNUXSITDOWN,
    E_END_DECOR_KNUXSITDOWNG,
    E_END_DECOR_MIGHTYBREATHE,
    E_END_DECOR_MIGHTYCLINK,
    E_END_DECOR_MIGHTYCLINKG,
    E_END_DECOR_MIGHTYSITDOWN,
    E_END_DECOR_MIGHTYSITDOWNG,
    E_END_DECOR_MIGHTYSHOCKED,
    E_END_DECOR_TAILSSHOCKED,
    E_END_DECOR_TAILSSPIN,
    E_END_DECOR_STOOL_STILL,
    E_END_DECOR_KNUXICECREAMSHOCK,
    E_END_DECOR_MIGHTYICECREAMSHOCK,
    E_END_DECOR_LETTERBOX1,
    E_END_DECOR_LETTERBOX2,
} EncoreGoodEndDecorTypes;

typedef enum {
    E_END_KING,
    E_END_STOOLT,
    E_END_STOOLK,
    E_END_STOOLM,
    E_END_STOOLR,
    E_END_SONIC,
    E_END_TAILS,
    E_END_KNUX,
    E_END_MIGHTY,
    E_END_RAY,
    E_END_GLASSK,
    E_END_GLASSM,
    E_END_ICECREAM_CONE,
    E_END_ICECREAM_PINK,
    E_END_ICECREAM_GREEN,
    E_END_ICECREAM_PINK2,
    E_END_COUNT,
} EncoreGoodEndDecorIDs;

// Object Class
struct ObjectEncoreGoodEnd {
    RSDK_OBJECT
    int32 unused1;
    uint16 unused2;
    uint16 sfxSnap;
    int32 unused3;
    EntityDecoration *decorations[16];
};

// Entity Class
struct EntityEncoreGoodEnd {
    MANIA_CUTSCENE_BASE
    int32 unused1;
};

// Object Struct
extern ObjectEncoreGoodEnd *EncoreGoodEnd;

// Standard Entity Events
void EncoreGoodEnd_Update(void);
void EncoreGoodEnd_LateUpdate(void);
void EncoreGoodEnd_StaticUpdate(void);
void EncoreGoodEnd_Draw(void);
void EncoreGoodEnd_Create(void *data);
void EncoreGoodEnd_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void EncoreGoodEnd_EditorDraw(void);
void EncoreGoodEnd_EditorLoad(void);
#endif
void EncoreGoodEnd_Serialize(void);

// Extra Entity Functions
void EncoreGoodEnd_SetupDecorations(void);

void EncoreGoodEnd_SetupPlayer(int32 playerID);
void EncoreGoodEnd_StatePlayer_MoveToPos(void);
void EncoreGoodEnd_StatePlayer_EndingIdle(void);
void EncoreGoodEnd_StatePlayer_EndingSonic(void);

bool32 EncoreGoodEnd_Cutscene_MoveToPlace(EntityCutsceneSeq *host);
bool32 EncoreGoodEnd_Cutscene_WaitForMovementFinish(EntityCutsceneSeq *host);
bool32 EncoreGoodEnd_Cutscene_ClinkGlasses(EntityCutsceneSeq *host);
bool32 EncoreGoodEnd_Cutscene_KingAppear(EntityCutsceneSeq *host);
bool32 EncoreGoodEnd_Cutscene_ThanksForPlaying(EntityCutsceneSeq *host);
bool32 EncoreGoodEnd_Cutscene_FinishCutscene(EntityCutsceneSeq *host);

void EncoreGoodEnd_Cutscene_SkipCB(void);

#endif

#endif //! OBJ_ENCOREGOODEND_H
