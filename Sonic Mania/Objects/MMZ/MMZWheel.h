#ifndef OBJ_MMZWHEEL_H
#define OBJ_MMZWHEEL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectMMZWheel;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 amplitude;
    uint8 childCount;
    int32 speed;
    uint8 motionType;
    Vector2 center;
    Vector2 offset;
    int32 field_78;
    Animator animator;
} EntityMMZWheel;

// Object Struct
extern ObjectMMZWheel *MMZWheel;

// Standard Entity Events
void MMZWheel_Update(void);
void MMZWheel_LateUpdate(void);
void MMZWheel_StaticUpdate(void);
void MMZWheel_Draw(void);
void MMZWheel_Create(void* data);
void MMZWheel_StageLoad(void);
void MMZWheel_EditorDraw(void);
void MMZWheel_EditorLoad(void);
void MMZWheel_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MMZWHEEL_H
