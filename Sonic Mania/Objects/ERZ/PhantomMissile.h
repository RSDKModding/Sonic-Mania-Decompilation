#ifndef OBJ_PHANTOMMISSILE_H
#define OBJ_PHANTOMMISSILE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
} ObjectPhantomMissile;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 field_60;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    Vector2 drawPos;
    Entity *parent;
    Animator animator0;
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntityPhantomMissile;

// Object Struct
extern ObjectPhantomMissile *PhantomMissile;

// Standard Entity Events
void PhantomMissile_Update(void);
void PhantomMissile_LateUpdate(void);
void PhantomMissile_StaticUpdate(void);
void PhantomMissile_Draw(void);
void PhantomMissile_Create(void* data);
void PhantomMissile_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PhantomMissile_EditorDraw(void);
void PhantomMissile_EditorLoad(void);
#endif
void PhantomMissile_Serialize(void);

// Extra Entity Functions
void PhantomMissile_Unknown1(void);
void PhantomMissile_Unknown2(void);
void PhantomMissile_Unknown3(void);
void PhantomMissile_Unknown4(void);
void PhantomMissile_Unknown5(void);
void PhantomMissile_Unknown6(void);
void PhantomMissile_Unknown7(void);
void PhantomMissile_Unknown8(void);
void PhantomMissile_Unknown9(void);

#endif //!OBJ_PHANTOMMISSILE_H
