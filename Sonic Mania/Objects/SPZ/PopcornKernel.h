#ifndef OBJ_POPCORNKERNEL_H
#define OBJ_POPCORNKERNEL_H

#include "SonicMania.h"

// Object Class
struct ObjectPopcornKernel {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityPopcornKernel {
    RSDK_ENTITY
    StateMachine(state);
    int32 field_5C;
    int32 angleVel;
    int32 field_64;
    int32 field_68;
    Vector2 bounds;
    Animator animator;
};

// Object Struct
extern ObjectPopcornKernel *PopcornKernel;

// Standard Entity Events
void PopcornKernel_Update(void);
void PopcornKernel_LateUpdate(void);
void PopcornKernel_StaticUpdate(void);
void PopcornKernel_Draw(void);
void PopcornKernel_Create(void* data);
void PopcornKernel_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PopcornKernel_EditorDraw(void);
void PopcornKernel_EditorLoad(void);
#endif
void PopcornKernel_Serialize(void);

// Extra Entity Functions
void PopcornKernel_Unknown1(void);
void PopcornKernel_Unknown2(void);
void PopcornKernel_Unknown3(void);

#endif //!OBJ_POPCORNKERNEL_H
