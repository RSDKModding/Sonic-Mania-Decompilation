#ifndef OBJ_MSZCUTSCENEK_H
#define OBJ_MSZCUTSCENEK_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    uint16 playerFrames;
    uint16 sfxImpact;
    uint16 sfxDrop;
    Vector2 pos1;
    Vector2 pos2;
    Vector2 pos3;
    Entity *tornado;
    Entity *mystic;
} ObjectMSZCutsceneK;

// Entity Class
typedef struct {
	MANIA_CUTSCENE_BASE
} EntityMSZCutsceneK;

// Object Struct
extern ObjectMSZCutsceneK *MSZCutsceneK;

// Standard Entity Events
void MSZCutsceneK_Update(void);
void MSZCutsceneK_LateUpdate(void);
void MSZCutsceneK_StaticUpdate(void);
void MSZCutsceneK_Draw(void);
void MSZCutsceneK_Create(void* data);
void MSZCutsceneK_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void MSZCutsceneK_EditorDraw(void);
void MSZCutsceneK_EditorLoad(void);
#endif
void MSZCutsceneK_Serialize(void);

// Extra Entity Functions
void MSZCutsceneK_SkipCB(void);
void MSZCutsceneK_StartCutscene(void);
void MSZCutsceneK_SetupP2(int posX, int posY);

bool32 MSZCutsceneK_CutsceneState_Unknown1(EntityCutsceneSeq *host);
bool32 MSZCutsceneK_CutsceneState_Unknown2(EntityCutsceneSeq *host);

#endif //!OBJ_MSZCUTSCENEK_H
