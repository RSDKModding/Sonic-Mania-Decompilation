#ifndef OBJ_PUYOBEAN_H
#define OBJ_PUYOBEAN_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 fallDelays[5], { 16, 12, 8, 4, 2 });
    Hitbox hitbox;
    int32 field_20[2];
    int32 field_28[2];
    Entity *gameState[256];
    int32 field_430;
    Vector2 field_434[256];
    bool32 field_C34[84];
    int32 shinkDelay;
    Animator animator;
    uint16 aniFrames;
    uint16 sfxLand;
    uint16 sfxRotate;
    uint16 chainFrames[6];
    uint16 sfxJunk;
    uint16 sfxFall;
} ObjectPuyoBean;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 playerID;
    int32 type;
    int32 linkFlags;
    int32 field_68;
    Entity *linkBeans[4];
    int32 timer;
    int32 field_80;
    int32 moveTimer;
    int32 fallTimer;
    int32 fallDelay;
    int32 rotateSpeed;
    int32 field_94;
    int32 field_98;
    int32 idleTimer;
    int32 field_A0;
    int32 field_A4;
    Vector2 stillPos;
    int32 field_B0;
    int32 field_B4;
    Vector2 origin;
    int32 targetAngle;
    bool32 forceRotationFlag;
    bool32 flag;
    Entity *partner;
    StateMachine(stateInput);
    int32 controllerID;
    bool32 down;
    bool32 left;
    bool32 right;
    bool32 rotateRight;
    bool32 rotateLeft;
    bool32 forceRotateLeft;
    bool32 forceRotateRight;
    Animator animator;
#if RETRO_USE_TOUCH_CONTROLS
    int32 touchRight;
    int32 touchLeft;
#endif
} EntityPuyoBean;

// Object Struct
extern ObjectPuyoBean *PuyoBean;

// Standard Entity Events
void PuyoBean_Update(void);
void PuyoBean_LateUpdate(void);
void PuyoBean_StaticUpdate(void);
void PuyoBean_Draw(void);
void PuyoBean_Create(void* data);
void PuyoBean_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PuyoBean_EditorDraw(void);
void PuyoBean_EditorLoad(void);
#endif
void PuyoBean_Serialize(void);

// Extra Entity Functions
EntityPuyoBean *PuyoBean_GetPuyoBean(int playerID, int x, int y);
void PuyoBean_StateInput_HandlePlayerInputs(void);
void PuyoBean_Unknown2(void);
void PuyoBean_Unknown3(void);
void PuyoBean_CheckBeanLinks(EntityPuyoBean *bean, EntityPuyoBean *curLink);
void PuyoBean_HandleMoveBounds(void);
bool32 PuyoBean_Unknown6(EntityPuyoBean *bean);
void PuyoBean_CheckCollisions(void);
int PuyoBean_Unknown8(EntityPuyoBean *bean, int playerID, int x, int y);
int PuyoBean_Unknown9(EntityPuyoBean *bean, int playerID, int x, int y);
bool32 PuyoBean_Unknown10(int playerID, int x, int y);
void PuyoBean_Unknown11(bool32 useTempArray, int playerID, int x, int y);
uint8 PuyoBean_Unknown12(int x, int playerID, EntityPuyoBean *bean1, EntityPuyoBean *bean2);
void PuyoBean_CalculateStillPos(EntityPuyoBean *bean);
void PuyoBean_State_Idle(void);
void PuyoBean_State_Controlled(void);
void PuyoBean_State_GameState(void);
void PuyoBean_State_Falling(void);
void PuyoBean_Unknown18(void);
void PuyoBean_Unknown19(void);
void PuyoBean_Unknown20(void);
void PuyoBean_Unknown21(void);
void PuyoBean_Unknown22(void);
void PuyoBean_Unknown23(void);
void PuyoBean_Unknown24(void);

#endif //!OBJ_PUYOBEAN_H
