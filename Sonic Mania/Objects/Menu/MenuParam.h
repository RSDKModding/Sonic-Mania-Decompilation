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
    uint8 field_59;
    char menuTag[0x100];
    int32 selectionID;
    int32 field_160;
    int32 clearFlag;
    int32 field_168;
    int32 characterID;
    int32 zoneID;
    int32 actID;
    int32 dbRowID;
#if RETRO_USE_PLUS
    int32 isEncoreMode;
    int32 field_180;
    int32 viewReplay;
    int32 showGhost;
    uint8 field_18C;
    uint8 replayRank;
    uint8 field_18E;
    uint8 field_18F;
    int32 field_190;
    int32 field_194;
    int32 field_198;
    int32 field_19C;
#else
    int32 timeScore;
    int32 field_174;
    int32 field_178;
    int32 field_17C;
    int32 field_180;
#endif
    int32 vsZoneID;
    int32 vsActID;
    int32 field_1A8;
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
