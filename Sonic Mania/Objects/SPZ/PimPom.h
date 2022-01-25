#ifndef OBJ_PIMPOM_H
#define OBJ_PIMPOM_H

#include "SonicMania.h"

typedef enum {
    PIMPOM_SINGLE,
    PIMPOM_HORIZONTAL,
    PIMPOM_DIAGONAL,
    PIMPOM_VERTICAL,
} PimPomTypes;

typedef enum {
    PIMPOM_MOVE_NONE,
    PIMPOM_MOVE_NORMAL,
    PIMPOM_MOVE_CIRCLE,
    PIMPOM_MOVE_3,
    PIMPOM_MOVE_4,
} PimPomMoveTypes;

// Object Class
struct ObjectPimPom {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxPimPom;
    uint16 sfxBumper2;
};

// Entity Class
struct EntityPimPom {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateMove);
    int32 type;
    uint8 color;
    int32 angle2;
    uint8 spinSpeed;
    int32 negAngle;
    int32 timer;
    Hitbox hitbox;
    int32 moveType;
    Vector2 amplitude;
    int32 speed;
    int32 angleM;
    uint8 length;
    uint8 gap;
    uint8 numChildren;
    Vector2 drawPos;
    Vector2 offset;
    Vector2 moveAmount;
    Animator animator;
};

// Object Struct
extern ObjectPimPom *PimPom;

// Standard Entity Events
void PimPom_Update(void);
void PimPom_LateUpdate(void);
void PimPom_StaticUpdate(void);
void PimPom_Draw(void);
void PimPom_Create(void* data);
void PimPom_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PimPom_EditorDraw(void);
void PimPom_EditorLoad(void);
#endif
void PimPom_Serialize(void);

// Extra Entity Functions
void PimPom_State0_Unknown(void);
void PimPom_State1_Unknown(void);
void PimPom_State3_Unknown(void);

void PimPom_StateMove_None(void);
void PimPom_StateMove_Normal(void);
void PimPom_StateMove_Circle(void);
void PimPom_StateMove_ToTarget(void);
void PimPom_StateMove3_Unknown(void);

#endif //!OBJ_PIMPOM_H
