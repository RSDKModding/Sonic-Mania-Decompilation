#ifndef OBJ_UFO_SPRINGBOARD_H
#define OBJ_UFO_SPRINGBOARD_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    int32 field_8;
    int32 field_C;
    int32 field_10;
    int32 field_14;
    int32 field_18;
    uint16 field_1C;
    uint16 modelIndex;
    uint16 sceneIndex;
    uint16 field_22;
} ObjectUFO_Springboard;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 field_58;
    int32 height;
    uint8 rampCount;
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
    Matrix matrix1;
    Matrix matrix2;
    Matrix matrix3;
    Animator animator;
} EntityUFO_Springboard;

// Object Struct
extern ObjectUFO_Springboard *UFO_Springboard;

// Standard Entity Events
void UFO_Springboard_Update(void);
void UFO_Springboard_LateUpdate(void);
void UFO_Springboard_StaticUpdate(void);
void UFO_Springboard_Draw(void);
void UFO_Springboard_Create(void* data);
void UFO_Springboard_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UFO_Springboard_EditorDraw(void);
void UFO_Springboard_EditorLoad(void);
#endif
void UFO_Springboard_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UFO_SPRINGBOARD_H
