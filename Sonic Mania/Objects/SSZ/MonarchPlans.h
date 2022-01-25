#ifndef OBJ_MONARCHPLANS_H
#define OBJ_MONARCHPLANS_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
struct ObjectMonarchPlans {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
    uint16 field_10;
    uint16 meshIndex;
    uint16 sceneIndex;
};

// Entity Class
struct EntityMonarchPlans {
    RSDK_ENTITY
    int32 field_58;
    int32 field_5C;
    int32 field_60;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    int32 rotationZ;
    int32 rotationY;
    Matrix matrix1;
    Matrix matrix2;
    Matrix matrix3;
    Matrix matrix4;
};

// Object Struct
extern ObjectMonarchPlans *MonarchPlans;

// Standard Entity Events
void MonarchPlans_Update(void);
void MonarchPlans_LateUpdate(void);
void MonarchPlans_StaticUpdate(void);
void MonarchPlans_Draw(void);
void MonarchPlans_Create(void* data);
void MonarchPlans_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void MonarchPlans_EditorDraw(void);
void MonarchPlans_EditorLoad(void);
#endif
void MonarchPlans_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_MONARCHPLANS_H
