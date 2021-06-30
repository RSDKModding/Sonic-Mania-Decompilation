#ifndef OBJ_CIRCLEBUMPER_H
#define OBJ_CIRCLEBUMPER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    ushort aniFrames;
    ushort sfxBumper;
} ObjectCircleBumper;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(stateCollide);
    StateMachine(stateMove);
    int hitCount;
    int type;
    Vector2 amplitude;
    int speed;
    Vector2 originPos;
    Animator animator;
    int field_94;
} EntityCircleBumper;

// Object Struct
extern ObjectCircleBumper *CircleBumper;

// Standard Entity Events
void CircleBumper_Update(void);
void CircleBumper_LateUpdate(void);
void CircleBumper_StaticUpdate(void);
void CircleBumper_Draw(void);
void CircleBumper_Create(void *data);
void CircleBumper_StageLoad(void);
void CircleBumper_EditorDraw(void);
void CircleBumper_EditorLoad(void);
void CircleBumper_Serialize(void);

// Extra Entity Functions
void CircleBumper_DebugDraw(void);
void CircleBumper_DebugSpawn(void);

void CircleBumper_Unknown1(void);
void CircleBumper_Unknown2(void);
void CircleBumper_Unknown3(void);
void CircleBumper_Unknown4(void);
void CircleBumper_Unknown5(void);
void CircleBumper_Unknown6(void);
void CircleBumper_Unknown7(void);
void CircleBumper_Unknown8(void);
#endif //! OBJ_CIRCLEBUMPER_H
