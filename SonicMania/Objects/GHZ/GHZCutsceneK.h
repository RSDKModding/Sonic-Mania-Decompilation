#ifndef OBJ_GHZCUTSCENEK_H
#define OBJ_GHZCUTSCENEK_H

#include "Game.h"

// Object Class
struct ObjectGHZCutsceneK {
    RSDK_OBJECT
    int32 unused1;
};

// Entity Class
struct EntityGHZCutsceneK {
    MANIA_CUTSCENE_BASE
};

// Object Struct
extern ObjectGHZCutsceneK *GHZCutsceneK;

// Standard Entity Events
void GHZCutsceneK_Update(void);
void GHZCutsceneK_LateUpdate(void);
void GHZCutsceneK_StaticUpdate(void);
void GHZCutsceneK_Draw(void);
void GHZCutsceneK_Create(void *data);
void GHZCutsceneK_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void GHZCutsceneK_EditorDraw(void);
void GHZCutsceneK_EditorLoad(void);
#endif
void GHZCutsceneK_Serialize(void);

// Extra Entity Functions
bool32 GHZCutsceneK_Cutscene_None(void);

#endif //! OBJ_GHZCUTSCENEK_H
