#ifndef OBJ_DASETUP_H
#define OBJ_DASETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    bool32 initialized;
    int32 flag;
    int32 trackCount;
    int32 trackID;
    int32 activeTrack;
    Entity *control;
    Entity *labelA;
    Entity *labelB;
    Entity *fxFade;
    Entity *trackList[64];
    uint16 sfxEmerald;
    uint16 sfxMedal;
    uint16 sfxSSExit;
    uint16 sfxScoreTotal;
} ObjectDASetup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDASetup;

// Object Struct
extern ObjectDASetup *DASetup;

// Standard Entity Events
void DASetup_Update(void);
void DASetup_LateUpdate(void);
void DASetup_StaticUpdate(void);
void DASetup_Draw(void);
void DASetup_Create(void* data);
void DASetup_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void DASetup_EditorDraw(void);
void DASetup_EditorLoad(void);
#endif
void DASetup_Serialize(void);

// Extra Entity Functions
void DASetup_DisplayTrack(int32 trackID);
bool32 DASetup_HandleMedallionDebug(void);
void DASetup_SetupUI(void);
void DASetup_State_ManageControl(void);

#endif //!OBJ_DASETUP_H
