#ifndef OBJ_BEANSTALK_H
#define OBJ_BEANSTALK_H

#include "Game.h"

typedef enum {
    BEANSTALK_BEGIN,
    BEANSTALK_NODE,
    BEANSTALK_PLATFORM,
    BEANSTALK_CHOMPER,
} BeanstalkTypes;

// Object Class
struct ObjectBeanstalk {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxSeed;
    Hitbox hitboxPlatform;
    Hitbox hitboxPlant;
    uint16 sfxImpact;
    uint16 sfxBeanNode;
    uint16 sfxBeanChomp;
};

// Entity Class
struct EntityBeanstalk {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    int32 bezCtrlAngle;
    int32 bezCtrlLength;
    bool32 forceEnd;
    uint8 moveAngle;
    int32 timer;
    uint16 curBezierPos;
    int32 size;
    int32 bezierInc;
    int32 bezierPos;
    uint8 curNodeID;
    uint8 curDecorID;
    bool32 setupDecorNodeID;
    Vector2 startPos;
    Vector2 platformPos;
    int32 stoodAngle;
    int32 chomperOffset;
    uint8 activePlayers;
    Animator nodeAnimator;
    Animator plantAnimator;
    Animator fillerAnimator;
    bool32 startGrowth;
    bool32 shown;
    bool32 finished;
};

// Object Struct
extern ObjectBeanstalk *Beanstalk;

// Standard Entity Events
void Beanstalk_Update(void);
void Beanstalk_LateUpdate(void);
void Beanstalk_StaticUpdate(void);
void Beanstalk_Draw(void);
void Beanstalk_Create(void *data);
void Beanstalk_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Beanstalk_EditorDraw(void);
void Beanstalk_EditorLoad(void);
#endif
void Beanstalk_Serialize(void);

// Extra Entity Functions
int32 Beanstalk_GetNextNodeDistance(void);
int32 Beanstalk_GetRemainingDistance(void);
int32 Beanstalk_GetBezierInc(void);

void Beanstalk_DrawNodes(void);
void Beanstalk_DrawCreationNode(void);

void Beanstalk_HandleNodeMovement(void);
void Beanstalk_HandleNodeAppear(void);

void Beanstalk_CheckPlayerCollisions_Platform(void);
void Beanstalk_CheckPlayerCollisions_Chomper(void);

void Beanstalk_State_Init(void);
void Beanstalk_StateDirt_WaitForStart(void);
void Beanstalk_StateDirt_GrowthDelay(void);
void Beanstalk_StateDirt_Grow(void);

void Beanstalk_State_Node(void);
void Beanstalk_State_Platform(void);
void Beanstalk_State_Chomper(void);

#endif //! OBJ_BEANSTALK_H
