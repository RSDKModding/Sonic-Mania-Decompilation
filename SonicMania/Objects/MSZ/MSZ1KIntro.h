#ifndef OBJ_MSZ1KINTRO_H
#define OBJ_MSZ1KINTRO_H

#include "Game.h"

// Object Class
struct ObjectMSZ1KIntro {
    RSDK_OBJECT
    int32 unused;
    uint16 sfxImpact;
    uint16 playerFrames;
};

// Entity Class
struct EntityMSZ1KIntro {
    MANIA_CUTSCENE_BASE
};

// Object Struct
extern ObjectMSZ1KIntro *MSZ1KIntro;

// Standard Entity Events
void MSZ1KIntro_Update(void);
void MSZ1KIntro_LateUpdate(void);
void MSZ1KIntro_StaticUpdate(void);
void MSZ1KIntro_Draw(void);
void MSZ1KIntro_Create(void *data);
void MSZ1KIntro_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MSZ1KIntro_EditorDraw(void);
void MSZ1KIntro_EditorLoad(void);
#endif
void MSZ1KIntro_Serialize(void);

// Extra Entity Functions
bool32 MSZ1KIntro_Cutscene_SetupPlane(EntityCutsceneSeq *host);
bool32 MSZ1KIntro_Cutscene_MagicianMischief(EntityCutsceneSeq *host);
bool32 MSZ1KIntro_Cutscene_StartAct(EntityCutsceneSeq *host);

#endif //! OBJ_MSZ1KINTRO_H
