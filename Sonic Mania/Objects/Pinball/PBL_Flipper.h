#ifndef OBJ_PBL_FLIPPER_H
#define OBJ_PBL_FLIPPER_H

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
    Hitbox hitbox2;
    int32 drawType;
    uint16 field_30;
    uint16 aniFrames;
    uint16 sceneIndex;
    uint16 sfxFlipper;
    uint16 sfxFlipperStage;
} ObjectPBL_Flipper;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 minCraneID;
    int32 height;
    bool32 retractable;
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
    Matrix matrix1;
    Matrix matrix2;
    Matrix matrix3;
    bool32 buttonPress;
    bool32 buttonDown;
    bool32 field_170;
    Animator animator;
} EntityPBL_Flipper;

// Object Struct
extern ObjectPBL_Flipper *PBL_Flipper;

// Standard Entity Events
void PBL_Flipper_Update(void);
void PBL_Flipper_LateUpdate(void);
void PBL_Flipper_StaticUpdate(void);
void PBL_Flipper_Draw(void);
void PBL_Flipper_Create(void* data);
void PBL_Flipper_StageLoad(void);
void PBL_Flipper_EditorDraw(void);
void PBL_Flipper_EditorLoad(void);
void PBL_Flipper_Serialize(void);

// Extra Entity Functions
void PBL_Flipper_HandlePlayerInteractions(void);
void PBL_Flipper_Unknown2(void);
void PBL_Flipper_Unknown3(void);
void PBL_Flipper_Unknown4(void);

#endif

#endif //!OBJ_PBL_FLIPPER_H
