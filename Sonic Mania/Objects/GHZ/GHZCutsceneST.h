#ifndef OBJ_GHZCUTSCENEST_H
#define OBJ_GHZCUTSCENEST_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 unused1;
    Entity *fxRuby;
    Entity *claw;
    Entity *platform;
    Entity *phantomRuby;
    Entity *cutsceneHBH[5];
} ObjectGHZCutsceneST;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    bool32 activated;
    bool32 setupKnuxCutscene;
    int32 timer;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    Hitbox hitbox2;

    Hitbox hitbox;
} EntityGHZCutsceneST;

// Object Struct
extern ObjectGHZCutsceneST *GHZCutsceneST;

// Standard Entity Events
void GHZCutsceneST_Update(void);
void GHZCutsceneST_LateUpdate(void);
void GHZCutsceneST_StaticUpdate(void);
void GHZCutsceneST_Draw(void);
void GHZCutsceneST_Create(void* data);
void GHZCutsceneST_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void GHZCutsceneST_EditorDraw(void);
void GHZCutsceneST_EditorLoad(void);
#endif
void GHZCutsceneST_Serialize(void);

// Extra Entity Functions
void GHZCutsceneST_SetupObjects(void);
void GHZCutsceneST_SetupKnuxCutscene(void);
#if RETRO_USE_PLUS
void GHZCutsceneST_SkipCB(void);
#endif

bool32 GHZCutsceneST_Cutscene_FadeIn(EntityCutsceneSeq *host);
bool32 GHZCutsceneST_Cutscene_FinishRubyWarp(EntityCutsceneSeq *host);
bool32 GHZCutsceneST_Cutscene_ExitHBH(EntityCutsceneSeq *host);
bool32 GHZCutsceneST_Cutscene_SetupGHZ1(EntityCutsceneSeq *host);


#endif //!OBJ_GHZCUTSCENEST_H
