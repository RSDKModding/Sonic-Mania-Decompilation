#ifndef OBJ_FBZ1OUTRO_H
#define OBJ_FBZ1OUTRO_H

#include "Game.h"

// Object Class
struct ObjectFBZ1Outro {
    RSDK_OBJECT
    int32 unused1;
    uint16 sfxDrop;
    EntityBigSqueeze *bossBorderL;
    EntityBigSqueeze *bossBorderR;
    EntityBigSqueeze *bossManager;
    EntityCollapsingPlatform *collapsingPlatform;
    EntityCrane *craneP1;
    EntityCrane *craneP2;
};

// Entity Class
struct EntityFBZ1Outro {
    MANIA_CUTSCENE_BASE
    int32 grabbedPlayers;
};

// Object Struct
extern ObjectFBZ1Outro *FBZ1Outro;

// Standard Entity Events
void FBZ1Outro_Update(void);
void FBZ1Outro_LateUpdate(void);
void FBZ1Outro_StaticUpdate(void);
void FBZ1Outro_Draw(void);
void FBZ1Outro_Create(void *data);
void FBZ1Outro_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FBZ1Outro_EditorDraw(void);
void FBZ1Outro_EditorLoad(void);
#endif
void FBZ1Outro_Serialize(void);

// Extra Entity Functions
void FBZ1Outro_StartCutscene(void);
void FBZ1Outro_HandleTrash(void);
void FBZ1Outro_DispenseTrash(void);

bool32 FBZ1Outro_Cutscene_CrushTrash(EntityCutsceneSeq *host);
bool32 FBZ1Outro_Cutscene_TrashDrop(EntityCutsceneSeq *host);
bool32 FBZ1Outro_Cutscene_CraneRide(EntityCutsceneSeq *host);
bool32 FBZ1Outro_Cutscene_PrepareFBZ2(EntityCutsceneSeq *host);

#endif //! OBJ_FBZ1OUTRO_H
