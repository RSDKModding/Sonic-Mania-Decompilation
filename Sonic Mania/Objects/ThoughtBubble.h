#ifndef OBJ_THOUGHTBUBBLE_H
#define OBJ_THOUGHTBUBBLE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    ushort dustFrames;
} ObjectThoughtBubble;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int type;
    int timer;
    Vector2 drawPos1;
    Vector2 drawPos2;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
} EntityThoughtBubble;

// Object Struct
extern ObjectThoughtBubble *ThoughtBubble;

// Standard Entity Events
void ThoughtBubble_Update(void);
void ThoughtBubble_LateUpdate(void);
void ThoughtBubble_StaticUpdate(void);
void ThoughtBubble_Draw(void);
void ThoughtBubble_Create(void* data);
void ThoughtBubble_StageLoad(void);
void ThoughtBubble_EditorDraw(void);
void ThoughtBubble_EditorLoad(void);
void ThoughtBubble_Serialize(void);

// Extra Entity Functions
void ThoughtBubble_Unknown1(void);
void ThoughtBubble_Unknown2(void);
void ThoughtBubble_Unknown3(void);
void ThoughtBubble_Unknown4(void);
void ThoughtBubble_Unknown5(void);

#endif //!OBJ_THOUGHTBUBBLE_H
