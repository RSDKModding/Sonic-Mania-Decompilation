#ifndef OBJ_CHECKERBALL_H
#define OBJ_CHECKERBALL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    Hitbox innerBox;
    Hitbox outerBox;
    byte array1[24];
    sbyte array2[24];
    ushort aniFrames;
} ObjectCheckerBall;

// Entity Class
typedef struct {
    RSDK_ENTITY
    bool32 flag;
    int field_5C;
    int field_60;
    Vector2 roundedPos;
    Vector2 collisionOffset;
    byte activePlayers;
    Animator animator;
} EntityCheckerBall;

// Object Struct
extern ObjectCheckerBall *CheckerBall;

// Standard Entity Events
void CheckerBall_Update(void);
void CheckerBall_LateUpdate(void);
void CheckerBall_StaticUpdate(void);
void CheckerBall_Draw(void);
void CheckerBall_Create(void* data);
void CheckerBall_StageLoad(void);
void CheckerBall_EditorDraw(void);
void CheckerBall_EditorLoad(void);
void CheckerBall_Serialize(void);

// Extra Entity Functions
void CheckerBall_DebugSpawn(void);
void CheckerBall_DebugDraw(void);

void CheckerBall_Unknown1(void);
void CheckerBall_Unknown2(void);
void CheckerBall_Unknown3(void);
void CheckerBall_BadnikBreak(void *b, Hitbox *hitbox);
void CheckerBall_Unknown5(void);

#endif //!OBJ_CHECKERBALL_H
