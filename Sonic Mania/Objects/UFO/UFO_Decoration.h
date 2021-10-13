#ifndef OBJ_UFO_DECORATION_H
#define OBJ_UFO_DECORATION_H

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
    int32 drawType;
    uint8 field_20;
    uint8 field_21;
    uint16 modelIndices[8];
    uint16 sceneIndex;
} ObjectUFO_Decoration;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 field_58;
    int32 type;
    int32 size;
    int32 height;
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
    Animator animator;
} EntityUFO_Decoration;

// Object Struct
extern ObjectUFO_Decoration *UFO_Decoration;

// Standard Entity Events
void UFO_Decoration_Update(void);
void UFO_Decoration_LateUpdate(void);
void UFO_Decoration_StaticUpdate(void);
void UFO_Decoration_Draw(void);
void UFO_Decoration_Create(void* data);
void UFO_Decoration_StageLoad(void);
void UFO_Decoration_EditorDraw(void);
void UFO_Decoration_EditorLoad(void);
void UFO_Decoration_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UFO_DECORATION_H
