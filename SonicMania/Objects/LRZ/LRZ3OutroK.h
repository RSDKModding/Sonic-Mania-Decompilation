#ifndef OBJ_LRZ3OUTROK_H
#define OBJ_LRZ3OUTROK_H

#include "Game.h"

// Object Class
struct ObjectLRZ3OutroK {
    RSDK_OBJECT
    int32 unused; // possibly aniFrames in the RSDK editor??? who knows...
    uint16 sfxWarp;
    EntityEggPrison *prison;
    EntitySkyTeleporter *teleporter;
    EntityFXRuby *fxRuby;
};

// Entity Class
struct EntityLRZ3OutroK {
    MANIA_CUTSCENE_BASE
    Vector2 playerPos[2];
};

// Object Struct
extern ObjectLRZ3OutroK *LRZ3OutroK;

// Standard Entity Events
void LRZ3OutroK_Update(void);
void LRZ3OutroK_LateUpdate(void);
void LRZ3OutroK_StaticUpdate(void);
void LRZ3OutroK_Draw(void);
void LRZ3OutroK_Create(void *data);
void LRZ3OutroK_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void LRZ3OutroK_EditorDraw(void);
void LRZ3OutroK_EditorLoad(void);
#endif
void LRZ3OutroK_Serialize(void);

// Extra Entity Functions
void LRZ3OutroK_StartCutscene(void);

bool32 LRZ3OutroK_Cutscene_RunToTeleporter(EntityCutsceneSeq *host);
bool32 LRZ3OutroK_Cutscene_LandOnTeleporter(EntityCutsceneSeq *host);
bool32 LRZ3OutroK_Cutscene_UseTeleporter(EntityCutsceneSeq *host);
bool32 LRZ3OutroK_Cutscene_TeleporterActivated(EntityCutsceneSeq *host);

#endif //! OBJ_LRZ3OUTROK_H
