#ifndef OBJ_LRZ3OUTROK_H
#define OBJ_LRZ3OUTROK_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    uint16 sfxWarp;
    EntityEggPrison *prison;
    Entity *teleporter;
    EntityFXRuby *fxRuby;
} ObjectLRZ3OutroK;

// Entity Class
typedef struct {
	MANIA_CUTSCENE_BASE
    Vector2 playerPos[2];
} EntityLRZ3OutroK;

// Object Struct
extern ObjectLRZ3OutroK *LRZ3OutroK;

// Standard Entity Events
void LRZ3OutroK_Update(void);
void LRZ3OutroK_LateUpdate(void);
void LRZ3OutroK_StaticUpdate(void);
void LRZ3OutroK_Draw(void);
void LRZ3OutroK_Create(void* data);
void LRZ3OutroK_StageLoad(void);
void LRZ3OutroK_EditorDraw(void);
void LRZ3OutroK_EditorLoad(void);
void LRZ3OutroK_Serialize(void);

// Extra Entity Functions
void LRZ3OutroK_StartCutscene(void);

bool32 LRZ3OutroK_CutsceneState_Unknown1(EntityCutsceneSeq *host);
bool32 LRZ3OutroK_CutsceneState_Unknown2(EntityCutsceneSeq *host);
bool32 LRZ3OutroK_CutsceneState_Unknown3(EntityCutsceneSeq *host);
bool32 LRZ3OutroK_CutsceneState_Unknown4(EntityCutsceneSeq *host);

#endif //!OBJ_LRZ3OUTROK_H
