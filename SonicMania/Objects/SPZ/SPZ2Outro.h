#ifndef OBJ_SPZ2OUTRO_H
#define OBJ_SPZ2OUTRO_H

#include "Game.h"

// Object Class
struct ObjectSPZ2Outro {
    RSDK_OBJECT
    bool32 ignoreP2;
    int32 unused;
    EntityTVFlyingBattery *tvFlyingBattery;
    EntityWeatherTV *weatherTV;
    EntityEggPrison *prison;
};

// Entity Class
struct EntitySPZ2Outro {
    MANIA_CUTSCENE_BASE
};

// Object Struct
extern ObjectSPZ2Outro *SPZ2Outro;

// Standard Entity Events
void SPZ2Outro_Update(void);
void SPZ2Outro_LateUpdate(void);
void SPZ2Outro_StaticUpdate(void);
void SPZ2Outro_Draw(void);
void SPZ2Outro_Create(void *data);
void SPZ2Outro_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SPZ2Outro_EditorDraw(void);
void SPZ2Outro_EditorLoad(void);
#endif
void SPZ2Outro_Serialize(void);

// Extra Entity Functions
void SPZ2Outro_StartCutscene(void);

bool32 SPZ2Outro_Cutscene_SetupFBZTV(EntityCutsceneSeq *host);
bool32 SPZ2Outro_Cutscene_ExitStageRight(EntityCutsceneSeq *host);

void SPZ2Outro_DrawHook_PrepareWeatherTV(void);

bool32 SPZ2Outro_Cutscene_AsSeenOnTV(EntityCutsceneSeq *host);
bool32 SPZ2Outro_Cutscene_FBZFlyAway(EntityCutsceneSeq *host);

#endif //! OBJ_SPZ2OUTRO_H
