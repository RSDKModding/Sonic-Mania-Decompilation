#ifndef OBJ_PBL_BUMPER_H
#define OBJ_PBL_BUMPER_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    int32 field_8;
    int32 field_C;
    int32 field_10;
    int32 field_14;
    int32 field_18;
    Hitbox hitbox;
    uint16 field_24;
    uint16 modelFrames;
    uint16 sceneIndex;
    uint16 sfxBumper;
} ObjectPBL_Bumper;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 height;
    int32 field_60;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    int32 field_7C;
    int32 field_80;
    int32 field_84;
    int32 field_88;
    int32 field_8C;
    int32 field_90;
    int32 field_94;
    int32 field_98;
    int32 field_9C;
    int32 field_A0;
    int32 field_A4;
    int32 field_A8;
    Matrix matrix2;
    Matrix matrix1;
    Matrix matrix3;
    Animator animator;
} EntityPBL_Bumper;

// Object Struct
extern ObjectPBL_Bumper *PBL_Bumper;

// Standard Entity Events
void PBL_Bumper_Update(void);
void PBL_Bumper_LateUpdate(void);
void PBL_Bumper_StaticUpdate(void);
void PBL_Bumper_Draw(void);
void PBL_Bumper_Create(void* data);
void PBL_Bumper_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PBL_Bumper_EditorDraw(void);
void PBL_Bumper_EditorLoad(void);
#endif
void PBL_Bumper_Serialize(void);

// Extra Entity Functions
void PBL_Bumper_HandlePlayerInteractions(void);
void PBL_Bumper_Unknown2(void);
void PBL_Bumper_Unknown3(void);
void PBL_Bumper_Unknown4(void);

#endif

#endif //!OBJ_PBL_BUMPER_H
