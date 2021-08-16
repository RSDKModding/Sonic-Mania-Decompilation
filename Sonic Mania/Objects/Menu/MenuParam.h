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
    byte selectionFlag;
    byte field_59;
    char menuTag[0x100];
    int selectionID;
    int field_160;
    int clearFlag;
    int field_168;
    int characterID;
    int zoneID;
    int actID;
    int dbRowID;
#if RETRO_USE_PLUS
    int isEncoreMode;
    int field_180;
    int viewReplay;
    int showGhost;
    byte field_18C;
    byte replayRank;
    byte field_18E;
    byte field_18F;
    int field_190;
    int field_194;
    int field_198;
    int field_19C;
#else
    int timeScore;
    int field_174;
    int field_178;
    int field_17C;
    int field_180;
#endif
    int vsZoneID;
    int vsActID;
    int field_1A8;
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
void MenuParam_EditorDraw(void);
void MenuParam_EditorLoad(void);
void MenuParam_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MENUPARAM_H
