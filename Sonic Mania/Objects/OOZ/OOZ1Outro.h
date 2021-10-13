#ifndef OBJ_OOZ1OUTRO_H
#define OBJ_OOZ1OUTRO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    uint16 sfxDrop;
} ObjectOOZ1Outro;

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
    int32 boundsL;
    int32 boundsR;
    int32 boundsT;
    int32 boundsB;
} EntityOOZ1Outro;

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
bool32 OOZ1Outro_Unknown1(EntityCutsceneSeq *host);
bool32 OOZ1Outro_Unknown2(EntityCutsceneSeq *host);
bool32 OOZ1Outro_Unknown3(EntityCutsceneSeq *host);
bool32 OOZ1Outro_Unknown4(EntityCutsceneSeq *host);

#endif //!OBJ_OOZ1OUTRO_H
