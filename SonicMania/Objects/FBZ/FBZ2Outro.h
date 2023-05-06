#ifndef OBJ_FBZ2OUTRO_H
#define OBJ_FBZ2OUTRO_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectFBZ2Outro {
    RSDK_OBJECT
};

// Entity Class
struct EntityFBZ2Outro {
    RSDK_ENTITY
    int32 unused1;
};

// Object Struct
extern ObjectFBZ2Outro *FBZ2Outro;

// Standard Entity Events
void FBZ2Outro_Update(void);
void FBZ2Outro_LateUpdate(void);
void FBZ2Outro_StaticUpdate(void);
void FBZ2Outro_Draw(void);
void FBZ2Outro_Create(void *data);
void FBZ2Outro_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FBZ2Outro_EditorDraw(void);
void FBZ2Outro_EditorLoad(void);
#endif
void FBZ2Outro_Serialize(void);

// Extra Entity Functions
void FBZ2Outro_StartCutscene(EntityFBZ2Outro *outro);

bool32 FBZ2Outro_Cutscene_SetupGliders(EntityCutsceneSeq *host);
bool32 FBZ2Outro_Cutscene_RunToGlider(EntityCutsceneSeq *host);
bool32 FBZ2Outro_Cutscene_GlideAway(EntityCutsceneSeq *host);

#endif

#endif //! OBJ_FBZ2OUTRO_H
