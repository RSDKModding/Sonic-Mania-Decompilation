#ifndef OBJ_FBZ1OUTRO_H
#define OBJ_FBZ1OUTRO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    ushort sfxDrop;
    EntityBigSqueeze *boss1;
    EntityBigSqueeze *boss2;
    EntityBigSqueeze *boss3;
    EntityCollapsingPlatform *collapsingPlatform;
    EntityCrane *craneP1;
    EntityCrane *craneP2;
} ObjectFBZ1Outro;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    bool32 activated;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    Hitbox hitbox;
    int grabbedPlayers;
} EntityFBZ1Outro;

// Object Struct
extern ObjectFBZ1Outro *FBZ1Outro;

// Standard Entity Events
void FBZ1Outro_Update(void);
void FBZ1Outro_LateUpdate(void);
void FBZ1Outro_StaticUpdate(void);
void FBZ1Outro_Draw(void);
void FBZ1Outro_Create(void* data);
void FBZ1Outro_StageLoad(void);
void FBZ1Outro_EditorDraw(void);
void FBZ1Outro_EditorLoad(void);
void FBZ1Outro_Serialize(void);

// Extra Entity Functions
void FBZ1Outro_StartCutscene(void);
void FBZ1Outro_Unknown2(void);
void FBZ1Outro_DispenseTrash(void);

bool32 FBZ1Outro_CutsceneState_Unknown1(EntityCutsceneSeq *host);
bool32 FBZ1Outro_CutsceneState_Unknown2(EntityCutsceneSeq *host);
bool32 FBZ1Outro_CutsceneState_Unknown3(EntityCutsceneSeq *host);
bool32 FBZ1Outro_CutsceneState_Unknown4(EntityCutsceneSeq *host);

#endif //!OBJ_FBZ1OUTRO_H
