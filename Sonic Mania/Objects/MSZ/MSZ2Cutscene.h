#ifndef OBJ_MSZ2CUTSCENE_H
#define OBJ_MSZ2CUTSCENE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    EntityGiantPistol *pistol;
    Entity *parallaxSprite;
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
void MSZ2Cutscene_Create(void* data);
void MSZ2Cutscene_StageLoad(void);
void MSZ2Cutscene_EditorDraw(void);
void MSZ2Cutscene_EditorLoad(void);
void MSZ2Cutscene_Serialize(void);

// Extra Entity Functions
void MSZ2Cutscene_SetupCutscene(void);
void MSZ2Cutscene_GetPistolPtr(void);

bool32 MSZ2Cutscene_CutsceneState_Unknown1(EntityCutsceneSeq *host);
bool32 MSZ2Cutscene_CutsceneState_Unknown2(EntityCutsceneSeq *host);
bool32 MSZ2Cutscene_CutsceneState_Unknown3(EntityCutsceneSeq *host);
bool32 MSZ2Cutscene_CutsceneState_Unknown4(EntityCutsceneSeq *host);

#endif //!OBJ_MSZ2CUTSCENE_H
