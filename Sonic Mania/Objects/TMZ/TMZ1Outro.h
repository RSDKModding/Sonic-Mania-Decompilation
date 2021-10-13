#ifndef OBJ_TMZ1OUTRO_H
#define OBJ_TMZ1OUTRO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
} ObjectTMZ1Outro;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    int32 field_60;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    int32 field_7C;
} EntityTMZ1Outro;

// Object Struct
extern ObjectTMZ1Outro *TMZ1Outro;

// Standard Entity Events
void TMZ1Outro_Update(void);
void TMZ1Outro_LateUpdate(void);
void TMZ1Outro_StaticUpdate(void);
void TMZ1Outro_Draw(void);
void TMZ1Outro_Create(void* data);
void TMZ1Outro_StageLoad(void);
void TMZ1Outro_EditorDraw(void);
void TMZ1Outro_EditorLoad(void);
void TMZ1Outro_Serialize(void);

// Extra Entity Functions
bool32 TMZ1Outro_Cutscene1_State1(EntityCutsceneSeq *host);
bool32 TMZ1Outro_Cutscene1_State2(EntityCutsceneSeq *host);
bool32 TMZ1Outro_Cutscene1_State3(EntityCutsceneSeq *host);
bool32 TMZ1Outro_Cutscene1_State4(EntityCutsceneSeq *host);

bool32 TMZ1Outro_Cutscene2_State1(EntityCutsceneSeq *host);

#endif //!OBJ_TMZ1OUTRO_H
