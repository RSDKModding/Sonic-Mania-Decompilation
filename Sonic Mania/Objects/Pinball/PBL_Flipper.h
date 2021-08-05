#ifndef OBJ_PBL_FLIPPER_H
#define OBJ_PBL_FLIPPER_H

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
    Hitbox hitbox2;
    int drawType;
    ushort field_30;
    ushort aniFrames;
    ushort sceneIndex;
    ushort sfxFlipper;
    ushort sfxFlipperStage;
} ObjectPBL_Flipper;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    byte minCraneID;
    int height;
    bool32 retractable;
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
