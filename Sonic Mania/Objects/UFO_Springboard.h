#ifndef OBJ_UFO_SPRINGBOARD_H
#define OBJ_UFO_SPRINGBOARD_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    ushort field_1C;
    ushort modelIndex;
    ushort sceneIndex;
    ushort field_22;
} ObjectUFO_Springboard;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int field_58;
    int height;
    byte rampCount;
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
void UFO_Springboard_EditorDraw(void);
void UFO_Springboard_EditorLoad(void);
void UFO_Springboard_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UFO_SPRINGBOARD_H
