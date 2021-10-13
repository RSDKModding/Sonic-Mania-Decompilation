#ifndef OBJ_TTCUTSCENE_H
#define OBJ_TTCUTSCENE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    EntityFXFade *fxFade;
} ObjectTTCutscene;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    bool32 activated;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    Hitbox hitbox;
} EntityTTCutscene;

// Object Struct
extern ObjectTTCutscene *TTCutscene;

// Standard Entity Events
void TTCutscene_Update(void);
void TTCutscene_LateUpdate(void);
void TTCutscene_StaticUpdate(void);
void TTCutscene_Draw(void);
void TTCutscene_Create(void* data);
void TTCutscene_StageLoad(void);
void TTCutscene_EditorDraw(void);
void TTCutscene_EditorLoad(void);
void TTCutscene_Serialize(void);

// Extra Entity Functions
void TTCutscene_StartCutscene(void);
void TTCutscene_SkipCB(void);

bool32 TTCutscene_CutsceneState_Setup(EntityCutsceneSeq *host);
bool32 TTCutscene_CutsceneState_FlyIn(EntityCutsceneSeq *host);
bool32 TTCutscene_CutsceneState_Wait(EntityCutsceneSeq *host);
bool32 TTCutscene_CutsceneState_FlyOut(EntityCutsceneSeq *host);
bool32 TTCutscene_CutsceneState_NextScene(EntityCutsceneSeq *host);

#endif //!OBJ_TTCUTSCENE_H
