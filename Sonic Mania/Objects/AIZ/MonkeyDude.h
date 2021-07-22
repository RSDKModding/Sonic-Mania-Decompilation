#ifndef OBJ_MONKEYDUDE_H
#define OBJ_MONKEYDUDE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    Hitbox coconutHitbox;
    Animator bodyData;
    Animator mainData;
    Animator coconutData;
    ushort aniFrames;
    ushort sfxDrop;
} ObjectMonkeyDude;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
    int activeParts;
    int currentAngle;
    void (*bodyStates[4])(void);
    int bodyAngles[4];
    int bodyUnknown2[4];
    int bodyUnknown[4];
    byte bodyPartID;
    Vector2 startPos;
    byte startDir;
    int drawY;
    int moveCount;
    int nummoves;
    int throwCount;
    int angleSpeed;
    Animator coconutData;
    Animator handData;
    int field_FC;
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
void MonkeyDude_State_Coconut(void);

#endif //!OBJ_MONKEYDUDE_H
