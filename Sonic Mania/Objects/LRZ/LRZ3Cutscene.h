#ifndef OBJ_LRZ3CUTSCENE_H
#define OBJ_LRZ3CUTSCENE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
} ObjectLRZ3Cutscene;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int field_58;
} EntityLRZ3Cutscene;

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
bool32 LRZ3Cutscene_Unknown1(EntityCutsceneSeq *host);
bool32 LRZ3Cutscene_Unknown2(EntityCutsceneSeq *host);

#endif //!OBJ_LRZ3CUTSCENE_H
