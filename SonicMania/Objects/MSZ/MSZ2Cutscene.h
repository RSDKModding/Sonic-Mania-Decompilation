#ifndef OBJ_MSZ2CUTSCENE_H
#define OBJ_MSZ2CUTSCENE_H

#include "Game.h"
#include "GiantPistol.h"
#include "Common/ParallaxSprite.h"
#include "Cutscene/CutsceneSeq.h"
#include "Global/EggPrison.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 unused;
    EntityGiantPistol *pistol;
    EntityParallaxSprite *oozPeek;
    EntityEggPrison *prison;
} ObjectMSZ2Cutscene;

// Entity Class
typedef struct {
    MANIA_CUTSCENE_BASE
} EntityMSZ2Cutscene;

// Object Struct
extern ObjectMSZ2Cutscene *MSZ2Cutscene;

// Standard Entity Events
void MSZ2Cutscene_Update(void);
void MSZ2Cutscene_LateUpdate(void);
void MSZ2Cutscene_StaticUpdate(void);
void MSZ2Cutscene_Draw(void);
void MSZ2Cutscene_Create(void *data);
void MSZ2Cutscene_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MSZ2Cutscene_EditorDraw(void);
void MSZ2Cutscene_EditorLoad(void);
#endif
void MSZ2Cutscene_Serialize(void);

// Extra Entity Functions
void MSZ2Cutscene_SetupCutscene(void);
void MSZ2Cutscene_GetPistolPtr(void);

bool32 MSZ2Cutscene_Cutscene_GoToPistol(EntityCutsceneSeq *host);
bool32 MSZ2Cutscene_Cutscene_EnterPistol(EntityCutsceneSeq *host);
bool32 MSZ2Cutscene_Cutscene_PistolFired(EntityCutsceneSeq *host);
bool32 MSZ2Cutscene_Cutscene_AppearInBG(EntityCutsceneSeq *host);

#endif //! OBJ_MSZ2CUTSCENE_H
