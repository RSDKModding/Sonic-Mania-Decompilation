#ifndef OBJ_PSZ2OUTRO_H
#define OBJ_PSZ2OUTRO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
} ObjectPSZ2Outro;

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
    Hitbox hitbox;
    Entity *eggman;
    EntityPhantomRuby *ruby;
    EntityFXRuby *fxRuby;
} EntityPSZ2Outro;

// Object Struct
extern ObjectPSZ2Outro *PSZ2Outro;

// Standard Entity Events
void PSZ2Outro_Update(void);
void PSZ2Outro_LateUpdate(void);
void PSZ2Outro_StaticUpdate(void);
void PSZ2Outro_Draw(void);
void PSZ2Outro_Create(void *data);
void PSZ2Outro_StageLoad(void);
void PSZ2Outro_EditorDraw(void);
void PSZ2Outro_EditorLoad(void);
void PSZ2Outro_Serialize(void);

// Extra Entity Functions
bool32 PSZ2Outro_CutsceneState_Unknown1(EntityCutsceneSeq *host);
bool32 PSZ2Outro_CutsceneState_Unknown2(EntityCutsceneSeq *host);
bool32 PSZ2Outro_CutsceneState_Unknown3(EntityCutsceneSeq *host);
bool32 PSZ2Outro_CutsceneState_Unknown4(EntityCutsceneSeq *host);
bool32 PSZ2Outro_CutsceneState_Unknown5(EntityCutsceneSeq *host);
bool32 PSZ2Outro_CutsceneState_Unknown6(EntityCutsceneSeq *host);
bool32 PSZ2Outro_CutsceneState_LoadNextScene(EntityCutsceneSeq *host);

#endif //! OBJ_PSZ2OUTRO_H
