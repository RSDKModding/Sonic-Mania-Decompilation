#ifndef OBJ_ERZOUTRO_H
#define OBJ_ERZOUTRO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    int32 field_8;
    int32 field_C;
    Entity *fxRuby;
    Entity *king;
    Entity *eggman;
    Entity *rubyPortal;
    Entity *ruby;
    int32 field_24;
    int32 field_28;
    int32 field_2C;
    bool32 savedGame;
} ObjectERZOutro;

// Entity Class
typedef struct {
    MANIA_CUTSCENE_BASE
} EntityERZOutro;

// Object Struct
extern ObjectERZOutro *ERZOutro;

// Standard Entity Events
void ERZOutro_Update(void);
void ERZOutro_LateUpdate(void);
void ERZOutro_StaticUpdate(void);
void ERZOutro_Draw(void);
void ERZOutro_Create(void* data);
void ERZOutro_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void ERZOutro_EditorDraw(void);
void ERZOutro_EditorLoad(void);
#endif
void ERZOutro_Serialize(void);

// Extra Entity Functions
void ERZOutro_SetEmeraldStates(void);
void ERZOutro_HandleRubyHover(void);

bool32 ERZOutro_Cutscene_AttackEggman(EntityCutsceneSeq *host);
bool32 ERZOutro_Cutscene_AttackRecoil(EntityCutsceneSeq *host);
bool32 ERZOutro_Cutscene_LoseEmeralds(EntityCutsceneSeq *host);
bool32 ERZOutro_Cutscene_OpenPortal(EntityCutsceneSeq *host);
bool32 ERZOutro_Cutscene_EnterPortal(EntityCutsceneSeq *host);
bool32 ERZOutro_Cutscene_FadeOut(EntityCutsceneSeq *host);
bool32 ERZOutro_Cutscene_ShowEnding(EntityCutsceneSeq *host);

void ERZOutro_SaveFileCB(bool32 success);

#endif //!OBJ_ERZOUTRO_H
