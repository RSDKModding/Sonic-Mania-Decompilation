#ifndef OBJ_PBL_BUMPER_H
#define OBJ_PBL_BUMPER_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    Hitbox hitbox;
    ushort field_24;
    ushort modelFrames;
    ushort sceneIndex;
    ushort sfxBumper;
} ObjectPBL_Bumper;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int height;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
    int field_A8;
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
void PBL_Bumper_EditorDraw(void);
void PBL_Bumper_EditorLoad(void);
void PBL_Bumper_Serialize(void);

// Extra Entity Functions
void PBL_Bumper_HandlePlayerInteractions(void);
void PBL_Bumper_Unknown2(void);
void PBL_Bumper_Unknown3(void);
void PBL_Bumper_Unknown4(void);

#endif

#endif //!OBJ_PBL_BUMPER_H
