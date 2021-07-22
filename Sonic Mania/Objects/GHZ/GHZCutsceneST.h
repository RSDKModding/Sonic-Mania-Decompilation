#ifndef OBJ_GHZCUTSCENEST_H
#define OBJ_GHZCUTSCENEST_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
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
    int timer;
    int field_64;
    int timer2;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
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
void GHZCutsceneST_EditorDraw(void);
void GHZCutsceneST_EditorLoad(void);
void GHZCutsceneST_Serialize(void);

// Extra Entity Functions
void GHZCutsceneST_SetupCutscene(void);
bool32 GHZCutsceneST_CutsceneState_Unknown1(EntityGHZCutsceneST *host);
bool32 GHZCutsceneST_CutsceneState_Unknown2(EntityGHZCutsceneST *host);
bool32 GHZCutsceneST_CutsceneState_Unknown3(EntityGHZCutsceneST *host);
bool32 GHZCutsceneST_CutsceneState_LoadNextStage(EntityGHZCutsceneST *host);
void GHZCutsceneST_SkipCB(void);

#endif //!OBJ_GHZCUTSCENEST_H
