#ifndef OBJ_PHANTOMRUBY_H
#define OBJ_PHANTOMRUBY_H

#include "SonicMania.h"

typedef enum {
    RUBYSFX_ATTACK1 = 1,
    RUBYSFX_ATTACK2,
    RUBYSFX_ATTACK3,
    RUBYSFX_ATTACK4,
    RUBYSFX_ATTACK5,
    RUBYSFX_ATTACK6,
    RUBYSFX_REDCUBE,
} RubySFX;

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfx_L[7];
    uint16 sfx_R[7];
} ObjectPhantomRuby;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    int32 timer;
    int32 flag;
    int32 field_6C;
    int32 sfx;
    int32 field_74;
    int32 field_78;
    Animator animator1;
    Animator animator2;
} EntityPhantomRuby;

// Object Struct
extern ObjectPhantomRuby *PhantomRuby;

// Standard Entity Events
void PhantomRuby_Update(void);
void PhantomRuby_LateUpdate(void);
void PhantomRuby_StaticUpdate(void);
void PhantomRuby_Draw(void);
void PhantomRuby_Create(void* data);
void PhantomRuby_StageLoad(void);
void PhantomRuby_EditorDraw(void);
void PhantomRuby_EditorLoad(void);
void PhantomRuby_Serialize(void);

// Extra Entity Functions
void PhantomRuby_PlaySFX(uint8 sfxID);
void PhantomRuby_Unknown2(EntityPhantomRuby *ruby);
void PhantomRuby_Unknown3(void);
void PhantomRuby_Unknown4(void);
void PhantomRuby_Unknown5(void);
void PhantomRuby_Unknown6(void);
void PhantomRuby_Unknown7(void);
void PhantomRuby_Unknown8(void);
void PhantomRuby_Unknown9(void);
void PhantomRuby_Unknown10(void);

#endif //!OBJ_PHANTOMRUBY_H
