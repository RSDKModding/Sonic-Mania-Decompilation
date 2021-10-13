#ifndef OBJ_MONKEYDUDE_H
#define OBJ_MONKEYDUDE_H

#include "SonicMania.h"

#define MonkeyDude_MaxBodyParts (4)

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    Hitbox coconutHitbox;
    Animator bodyAnimator;
    Animator tailAnimator;
    Animator animator;
    uint16 aniFrames;
#if RETRO_USE_PLUS
    uint16 sfxDrop;
#endif
} ObjectMonkeyDude;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
    int activeParts;
    int currentAngle;
    void (*bodyStates[MonkeyDude_MaxBodyParts])(void);
    int bodyAngles[MonkeyDude_MaxBodyParts];
    int bodyFlags[MonkeyDude_MaxBodyParts];
    int bodyTimers[MonkeyDude_MaxBodyParts];
    uint8 bodyPartID;
    Vector2 startPos;
    uint8 startDir;
    int drawY;
    int moveCount;
    int nummoves;
    int throwCount;
    int angleSpeed;
    Animator animator;
    Animator handData;
    int coconutFrame;
} EntityMonkeyDude;

// Object Struct
extern ObjectMonkeyDude *MonkeyDude;

// Standard Entity Events
void MonkeyDude_Update(void);
void MonkeyDude_LateUpdate(void);
void MonkeyDude_StaticUpdate(void);
void MonkeyDude_Draw(void);
void MonkeyDude_Create(void* data);
void MonkeyDude_StageLoad(void);
void MonkeyDude_EditorDraw(void);
void MonkeyDude_EditorLoad(void);
void MonkeyDude_Serialize(void);

// Extra Entity Functions
void MonkeyDude_DebugDraw(void);
void MonkeyDude_DebugSpawn(void);

void MonkeyDude_State_Setup(void);
void MonkeyDude_HandleBodyPart(void);
void MonkeyDude_HandleStates(void);
void MonkeyDude_State2(void);
void MonkeyDude_State3(void);
void MonkeyDude_State4(void);
void MonkeyDude_State_BodyUnknown(void);
void MonkeyDude_State_BodyUnknown2(void);
#if RETRO_USE_PLUS
void MonkeyDude_State_Coconut(void);
#endif

#endif //!OBJ_MONKEYDUDE_H
