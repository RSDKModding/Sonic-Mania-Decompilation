#ifndef OBJ_UFO_DECORATION_H
#define OBJ_UFO_DECORATION_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int drawType;
    char field_20;
    char field_21;
    ushort modelIndices[8];
    ushort sceneIndex;
} ObjectUFO_Decoration;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int field_58;
    int type;
    int size;
    int height;
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
