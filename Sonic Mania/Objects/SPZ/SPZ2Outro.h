#ifndef OBJ_SPZ2OUTRO_H
#define OBJ_SPZ2OUTRO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    bool32 flag;
    int32 field_8;
    Entity *tvFlyingBattery;
    Entity *weatherTV;
    EntityEggPrison *prison;
} ObjectSPZ2Outro;

// Entity Class
typedef struct {
    MANIA_CUTSCENE_BASE
} EntitySPZ2Outro;

// Object Struct
extern ObjectSPZ2Outro *SPZ2Outro;

// Standard Entity Events
void SPZ2Outro_Update(void);
void SPZ2Outro_LateUpdate(void);
void SPZ2Outro_StaticUpdate(void);
void SPZ2Outro_Draw(void);
void SPZ2Outro_Create(void* data);
void SPZ2Outro_StageLoad(void);
void SPZ2Outro_EditorDraw(void);
void SPZ2Outro_EditorLoad(void);
void SPZ2Outro_Serialize(void);

// Extra Entity Functions
void SPZ2Outro_StartCutscene(void);

bool32 SPZ2Outro_CutsceneState_Unknown1(EntityCutsceneSeq *host);
bool32 SPZ2Outro_CutsceneState_Unknown2(EntityCutsceneSeq *host);

void SPZ2Outro_DrawLayerCB(void);

bool32 SPZ2Outro_CutsceneState_Unknown3(EntityCutsceneSeq *host);
bool32 SPZ2Outro_CutsceneState_Unknown4(EntityCutsceneSeq *host);

#endif //!OBJ_SPZ2OUTRO_H
