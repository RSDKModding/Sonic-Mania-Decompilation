#ifndef OBJ_OOZ1OUTRO_H
#define OBJ_OOZ1OUTRO_H

#include "SonicMania.h"

// Object Class
struct ObjectOOZ1Outro {
    RSDK_OBJECT
    int32 field_4;
    uint16 sfxDrop;
};

// Entity Class
struct EntityOOZ1Outro {
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
    int32 boundsL;
    int32 boundsR;
    int32 boundsT;
    int32 boundsB;
};

// Object Struct
extern ObjectOOZ1Outro *OOZ1Outro;

// Standard Entity Events
void OOZ1Outro_Update(void);
void OOZ1Outro_LateUpdate(void);
void OOZ1Outro_StaticUpdate(void);
void OOZ1Outro_Draw(void);
void OOZ1Outro_Create(void* data);
void OOZ1Outro_StageLoad(void);
void OOZ1Outro_EditorDraw(void);
void OOZ1Outro_EditorLoad(void);
void OOZ1Outro_Serialize(void);

// Extra Entity Functions
bool32 OOZ1Outro_Cutscene_FadeIn(EntityCutsceneSeq *host);
bool32 OOZ1Outro_Cutscene_PostActClearSetup(EntityCutsceneSeq *host);
bool32 OOZ1Outro_Cutscene_FallIntoAct2(EntityCutsceneSeq *host);
bool32 OOZ1Outro_Cutscene_BeginAct(EntityCutsceneSeq *host);

#endif //!OBJ_OOZ1OUTRO_H
