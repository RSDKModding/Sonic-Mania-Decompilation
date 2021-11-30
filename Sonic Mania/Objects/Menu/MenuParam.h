#ifndef OBJ_MENUPARAM_H
#define OBJ_MENUPARAM_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMenuParam;

// Entity Class
typedef struct {
    RSDK_ENTITY
    uint8 selectionFlag;
    uint8 selectionType;
    char menuTag[0x100];
    int32 selectionID;
    int32 startedTAAttempt; // only used in pre-plus, goes unused by the time plus rolled around
    int32 clearFlag;
    int32 replayID;
    int32 characterID;
    int32 zoneID;
    int32 actID;
    int32 timeAttackRank;
#if RETRO_USE_PLUS
    int32 isEncoreMode;
    int32 replayUUID;
    int32 viewReplay;
    int32 showGhost;
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
} EntityMenuParam;

// Object Struct
extern ObjectMenuParam *MenuParam;

// Standard Entity Events
void MenuParam_Update(void);
void MenuParam_LateUpdate(void);
void MenuParam_StaticUpdate(void);
void MenuParam_Draw(void);
void MenuParam_Create(void* data);
void MenuParam_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void MenuParam_EditorDraw(void);
void MenuParam_EditorLoad(void);
#endif
void MenuParam_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MENUPARAM_H
