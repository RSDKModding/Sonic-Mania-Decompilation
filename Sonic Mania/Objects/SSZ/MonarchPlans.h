#ifndef OBJ_MONARCHPLANS_H
#define OBJ_MONARCHPLANS_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
    uint16 field_10;
    uint16 meshIndex;
    uint16 sceneIndex;
} ObjectMonarchPlans;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int field_58;
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int rotationZ;
    int rotationY;
    Matrix matrix1;
    Matrix matrix2;
    Matrix matrix3;
    Matrix matrix4;
} EntityMonarchPlans;

// Object Struct
extern ObjectMonarchPlans *MonarchPlans;

// Standard Entity Events
void MonarchPlans_Update(void);
void MonarchPlans_LateUpdate(void);
void MonarchPlans_StaticUpdate(void);
void MonarchPlans_Draw(void);
void MonarchPlans_Create(void* data);
void MonarchPlans_StageLoad(void);
void MonarchPlans_EditorDraw(void);
void MonarchPlans_EditorLoad(void);
void MonarchPlans_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_MONARCHPLANS_H
