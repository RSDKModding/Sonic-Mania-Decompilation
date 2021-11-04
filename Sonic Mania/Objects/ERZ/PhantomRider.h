#ifndef OBJ_PHANTOMRIDER_H
#define OBJ_PHANTOMRIDER_H

#include "SonicMania.h"

typedef enum { PHANTOMRIDER_JIMMY = 3 } PhantomRiderTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxRiderLaunch;
    uint16 field_8;
} ObjectPhantomRider;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 startPos;
    int32 timer;
    int32 invincibilityTimer;
    int32 field_70;
    int32 type;
    bool32 prevOnGround;
    Entity *child;
    Hitbox hitbox;
    Hitbox outerBox;
    Hitbox innerBox;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Animator animator6;
} EntityPhantomRider;

// Object Struct
extern ObjectPhantomRider *PhantomRider;

// Standard Entity Events
void PhantomRider_Update(void);
void PhantomRider_LateUpdate(void);
void PhantomRider_StaticUpdate(void);
void PhantomRider_Draw(void);
void PhantomRider_Create(void* data);
void PhantomRider_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PhantomRider_EditorDraw(void);
void PhantomRider_EditorLoad(void);
#endif
void PhantomRider_Serialize(void);

// Extra Entity Functions
void PhantomRider_ProcessAutoScroll(void);

void PhantomRider_StateDraw_Unknown1(void);

void PhantomRider_State_Unknown1(void);
void PhantomRider_State_Unknown2(void);
void PhantomRider_State_Unknown3(void);
void PhantomRider_State_Unknown4(void);
void PhantomRider_State_Unknown5(void);

void PhantomRider_State2_Unknown(void);

#endif //!OBJ_PHANTOMRIDER_H
