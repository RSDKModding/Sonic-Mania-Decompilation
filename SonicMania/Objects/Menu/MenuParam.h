#ifndef OBJ_MENUPARAM_H
#define OBJ_MENUPARAM_H

#include "Game.h"

typedef enum {
    BSS_SELECTION_NONE,
    BSS_SELECTION_EXTRAS,
} BSSSelectionType;

typedef enum {
    PUYO_SELECTION_NONE,
    PUYO_SELECTION_VS_CPU,
    PUYO_SELECTION_VS_2P,
    PUYO_SELECTION_TIE_BREAKER,
} PuyoSelectionFlags;

typedef enum {
    CREDITS_SELECTION_NONE,
    CREDITS_SELECTION_EXTRAS,
} CreditsSelectionType;

typedef enum {
    EXTRAS_SELECTION_BSS,
    EXTRAS_SELECTION_PUYO,
    EXTRAS_SELECTION_DAGARDEN,
    EXTRAS_SELECTION_CREDITS,
} ExtrasSelectionIDs;

// Object Class
struct ObjectMenuParam {
    RSDK_OBJECT
};

// Entity Class
struct EntityMenuParam {
    RSDK_ENTITY
    uint8 puyoSelection;
    uint8 bssSelection;
    char menuTag[0x100];
    int32 menuSelection;
    bool32 startedTAAttempt; // only used in pre-plus, goes unused by the time plus rolled around
    bool32 inTimeAttack;
    int32 replayID;
    int32 characterID;
    int32 zoneID;
    int32 actID;
    int32 timeAttackRank;
#if MANIA_USE_PLUS
    bool32 isEncoreMode;
    int32 replayUUID;
    bool32 viewReplay;
    bool32 showGhost;
    uint8 selectedReplay;
    uint8 replayRankID;
#else
    int32 timeScore;
#endif
    int32 unused2;
    int32 unused3;
    int32 unused4;
    int32 unused5;
    int32 vsZoneID;
    int32 vsActID;
    int32 creditsReturnToMenu;
};

// Object Struct
extern ObjectMenuParam *MenuParam;

// Standard Entity Events
void MenuParam_Update(void);
void MenuParam_LateUpdate(void);
void MenuParam_StaticUpdate(void);
void MenuParam_Draw(void);
void MenuParam_Create(void *data);
void MenuParam_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void MenuParam_EditorDraw(void);
void MenuParam_EditorLoad(void);
#endif
void MenuParam_Serialize(void);

// Extra Entity Functions
EntityMenuParam *MenuParam_GetParam(void);

#endif //! OBJ_MENUPARAM_H
