#ifndef OBJ_SSZ1OUTRO_H
#define OBJ_SSZ1OUTRO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    EntityFXFade *fxFade;
    EntityRTeleporter *teleporter;
} ObjectSSZ1Outro;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    bool32 activated;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    Hitbox hitbox;
} EntitySSZ1Outro;

// Object Struct
extern ObjectSSZ1Outro *SSZ1Outro;

// Standard Entity Events
void SSZ1Outro_Update(void);
void SSZ1Outro_LateUpdate(void);
void SSZ1Outro_StaticUpdate(void);
void SSZ1Outro_Draw(void);
void SSZ1Outro_Create(void* data);
void SSZ1Outro_StageLoad(void);
void SSZ1Outro_EditorDraw(void);
void SSZ1Outro_EditorLoad(void);
void SSZ1Outro_Serialize(void);

// Extra Entity Functions
bool32 SSZ1Outro_CutsceneState_Unknown1(EntityCutsceneSeq *host);
bool32 SSZ1Outro_CutsceneState_Unknown2(EntityCutsceneSeq *host);

void SSZ1Outro_Unknown3(Entity *entity);
void SSZ1Outro_Unknown4(void);

#endif //!OBJ_SSZ1OUTRO_H
