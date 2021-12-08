#ifndef OBJ_BEANSTALK_H
#define OBJ_BEANSTALK_H

#include "SonicMania.h"

typedef enum {
    BEANSTALK_BEGIN,
    BEANSTALK_NODE,
    BEANSTALK_PLATFORM,
    BEANSTALK_CHOMPER,
}BeanstalkTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxSeed;
    Hitbox hitboxPlatform;
    Hitbox hitboxPlant;
    uint16 sfxImpact;
    uint16 sfxBeanNode;
    uint16 sfxBeanChomp;
} ObjectBeanstalk;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    int32 bezCtrlAngle;
    int32 bezCtrlLength;
    bool32 forceEnd;
    uint8 angle2;
    int32 timer;
    uint16 field_74;
    int32 field_78;
    int32 field_7C;
    int32 field_80;
    uint8 field_84;
    uint8 field_85;
    int32 field_88;
    Vector2 field_8C;
    Vector2 field_94;
    int32 field_9C;
    int32 field_A0;
    uint8 activePlayers;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    bool32 startGrowth;
    bool32 shown;
    bool32 flag2;
} EntityBeanstalk;

// Object Struct
extern ObjectBeanstalk *Beanstalk;

// Standard Entity Events
void Beanstalk_Update(void);
void Beanstalk_LateUpdate(void);
void Beanstalk_StaticUpdate(void);
void Beanstalk_Draw(void);
void Beanstalk_Create(void* data);
void Beanstalk_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Beanstalk_EditorDraw(void);
void Beanstalk_EditorLoad(void);
#endif
void Beanstalk_Serialize(void);

// Extra Entity Functions
int32 Beanstalk_Unknown1(void);
int32 Beanstalk_Unknown2(void);
int32 Beanstalk_Unknown3(void);
void Beanstalk_Unknown4(void);
void Beanstalk_Unknown5(void);
void Beanstalk_Unknown6(void);
void Beanstalk_Unknown7(void);

void Beanstalk_CheckPlayerCollisions_Platform(void);
void Beanstalk_CheckPlayerCollisions_Chomper(void);

void Beanstalk_State_Setup(void);
void Beanstalk_StateDirt_WaitForStart(void);
void Beanstalk_StateDirt_GrowthDelay(void);
void Beanstalk_StateDirt_Grow(void);

void Beanstalk_State_Node(void);
void Beanstalk_State_Platform(void);
void Beanstalk_State_Chomper(void);

#endif //!OBJ_BEANSTALK_H
