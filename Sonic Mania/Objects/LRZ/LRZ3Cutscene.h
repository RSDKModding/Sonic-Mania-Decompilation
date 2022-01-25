#ifndef OBJ_LRZ3CUTSCENE_H
#define OBJ_LRZ3CUTSCENE_H

#include "SonicMania.h"

// Object Class
struct ObjectLRZ3Cutscene {
    RSDK_OBJECT
    int32 field_4;
};

// Entity Class
struct EntityLRZ3Cutscene {
    RSDK_ENTITY
    int32 field_58;
};

// Object Struct
extern ObjectLRZ3Cutscene *LRZ3Cutscene;

// Standard Entity Events
void LRZ3Cutscene_Update(void);
void LRZ3Cutscene_LateUpdate(void);
void LRZ3Cutscene_StaticUpdate(void);
void LRZ3Cutscene_Draw(void);
void LRZ3Cutscene_Create(void* data);
void LRZ3Cutscene_StageLoad(void);
void LRZ3Cutscene_EditorDraw(void);
void LRZ3Cutscene_EditorLoad(void);
void LRZ3Cutscene_Serialize(void);

// Extra Entity Functions
bool32 LRZ3Cutscene_Cutscene_FadeIn(EntityCutsceneSeq *host);
bool32 LRZ3Cutscene_Cutscene_RunRight(EntityCutsceneSeq *host);

#endif //!OBJ_LRZ3CUTSCENE_H
