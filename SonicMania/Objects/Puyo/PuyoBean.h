#ifndef OBJ_PUYOBEAN_H
#define OBJ_PUYOBEAN_H

#include "Game.h"

#define PUYO_PLAYFIELD_W (6)
#define PUYO_PLAYFIELD_H (14)

typedef enum {
    PUYOBEAN_BLUE   = 0,
    PUYOBEAN_GREEN  = 6,
    PUYOBEAN_PURPLE = 12,
    PUYOBEAN_RED    = 18,
    PUYOBEAN_YELLOW = 24,
    PUYOBEAN_JUNK   = 30,
} PuyoBeanTypes;

typedef enum {
    PUYOBEAN_ANI_FLASH,
    PUYOBEAN_ANI_IDLE,
    PUYOBEAN_ANI_BOUNCE,
    PUYOBEAN_ANI_CONNECT,
    PUYOBEAN_ANI_POP,
    PUYOBEAN_ANI_DEBRIS,
} PuyoBeanAniIDs;

// Object Class
struct ObjectPuyoBean {
    RSDK_OBJECT
    TABLE(int32 fallDelays[5], { 16, 12, 8, 4, 2 });
    Hitbox hitboxBean;
    int32 comboChainCount[2];
    int32 disableBeanLink[2];
    EntityPuyoBean *playfield[256];
    int32 beanLinkCount;
    Vector2 beanLinkPositions[256];
    bool32 beanLinkTable[84]; // PUYO_PLAYFIELD_W * PUYO_PLAYFIELD_H
    int32 shinkDelay;
    Animator junkBeanAnimator;
    uint16 aniFrames;
    uint16 sfxLand;
    uint16 sfxRotate;
    uint16 chainFrames[6];
    uint16 sfxJunk;
    uint16 sfxFall;
};

// Entity Class
struct EntityPuyoBean {
    RSDK_ENTITY
    StateMachine(state);
    int32 playerID;
    int32 type;
    int32 linkSides;
    int32 linkCount;
    EntityPuyoBean *linkBeans[4];
    int32 timer;
    int32 popTimer;
    int32 moveTimer;
    int32 fallTimer;
    int32 fallDelay;
    int32 rotateSpeed;
    int32 unused1;
    int32 rotationDir;
    int32 idleTimer;
    int32 connectTimer;
    int32 selectedLevel;
    Vector2 stillPos;
    Vector2 unused2;
    Vector2 origin;
    int32 targetAngle;
    bool32 forceRotationActive;
    bool32 isJunk;
    EntityPuyoBean *partner;
    StateMachine(stateInput);
    int32 controllerID;
    bool32 down;
    bool32 left;
    bool32 right;
    bool32 rotateRight;
    bool32 rotateLeft;
    bool32 forceRotateLeft;
    bool32 forceRotateRight;
    Animator beanAnimator;
};

// Object Struct
extern ObjectPuyoBean *PuyoBean;

// Standard Entity Events
void PuyoBean_Update(void);
void PuyoBean_LateUpdate(void);
void PuyoBean_StaticUpdate(void);
void PuyoBean_Draw(void);
void PuyoBean_Create(void *data);
void PuyoBean_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PuyoBean_EditorDraw(void);
void PuyoBean_EditorLoad(void);
#endif
void PuyoBean_Serialize(void);

// Extra Entity Functions
EntityPuyoBean *PuyoBean_GetPuyoBean(int32 playerID, int32 x, int32 y);
void PuyoBean_Input_Player(void);
void PuyoBean_DestroyPuyoBeans(void);
void PuyoBean_HandleBeanLinks(void);
void PuyoBean_CheckBeanLinks(EntityPuyoBean *bean, EntityPuyoBean *curLink);
void PuyoBean_HandleMoveBounds(void);
bool32 PuyoBean_CheckAIRotationDisabled(EntityPuyoBean *bean);
void PuyoBean_CheckCollisions(void);
int32 PuyoBean_GetBeanChainRemovalCount(int32 playerID, EntityPuyoBean *bean, int32 x, int32 y);
int32 PuyoBean_GetAvaliableLinks(int32 playerID, EntityPuyoBean *bean, int32 x, int32 y);
bool32 PuyoBean_CheckLinkPosAvaliable(int32 playerID, int32 x, int32 y);
void PuyoBean_SetupBeanLinkTable(int32 playerID, int32 x, int32 y, bool32 useTempTable);
uint8 PuyoBean_GetColumnHeight(int32 playerID, int32 column, EntityPuyoBean *bean, EntityPuyoBean *partner);
void PuyoBean_CalculateStillPos(EntityPuyoBean *bean);

// States
void PuyoBean_State_PartnerControlled(void);
void PuyoBean_State_Controlled(void);
void PuyoBean_State_BeanIdle(void);
void PuyoBean_State_Falling(void);
void PuyoBean_State_BeanLand(void);
void PuyoBean_State_JunkLand(void);
void PuyoBean_State_JunkIdle(void);
void PuyoBean_State_JunkPopped(void);
void PuyoBean_State_BeginBeanPop(void);
void PuyoBean_State_BeanPop(void);
void PuyoBean_State_MatchLoseFall(void);

#endif //! OBJ_PUYOBEAN_H
