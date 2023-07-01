#ifndef OBJ_HEAVYMYSTIC_H
#define OBJ_HEAVYMYSTIC_H

#include "Game.h"

typedef enum {
    MYSTIC_MISCHIEF,
    MYSTIC_BOSS,
    MYSTIC_BOX,
    MYSTIC_CORK,
    MYSTIC_BOMB,
    MYSTIC_DEBRIS,
} HeavyMysticTypes;

// Object Class
struct ObjectHeavyMystic {
    RSDK_OBJECT
    int32 boundsL;
    int32 boundsR;
    int32 boundsM;
    int32 boundsT;
    int32 boundsB;
    int32 curtainLinePos;
    uint16 sfxAssemble;
    uint16 sfxMagicBox;
    uint16 sfxClack;
    uint16 sfxPowerDown;
    uint16 sfxImpact;
    uint16 sfxPon;
    uint16 sfxDrop;
    uint16 sfxImpact2;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxPoof;
    uint16 sfxTwinkle;
    uint16 sfxHat;
    uint16 sfxHatNode;
    uint16 sfxBleeps;
    uint16 sfxTransform;
    uint16 sfxAppear1;
    uint16 aniFrames;
    uint16 roguesFrames;
};

// Entity Class
struct EntityHeavyMystic {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 type;
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    int32 rogueHealth;
    int32 particleFXTimer;
    int32 unused;
    int32 attackID;
    int32 rogueID;
    Vector2 targetPos;
    Animator animator;
    Hitbox hitbox;
};

// Object Struct
extern ObjectHeavyMystic *HeavyMystic;

// Standard Entity Events
void HeavyMystic_Update(void);
void HeavyMystic_LateUpdate(void);
void HeavyMystic_StaticUpdate(void);
void HeavyMystic_Draw(void);
void HeavyMystic_Create(void *data);
void HeavyMystic_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void HeavyMystic_EditorDraw(void);
void HeavyMystic_EditorLoad(void);
#endif
void HeavyMystic_Serialize(void);

// Extra Entity Functions
void HeavyMystic_SpawnParticleFX(int32 x, int32 y);
void HeavyMystic_HandleParticleFX(void);

void HeavyMystic_CheckPlayerCollisions(void);
void HeavyMystic_Hit(void);
void HeavyMystic_CheckPlayerCollisions_Fang(void);
void HeavyMystic_CheckPlayerCollisions_Bark(void);
void HeavyMystic_CheckPlayerCollisions_Bean(void);
void HeavyMystic_Explode(void);

void HeavyMystic_Scanline_Curtains(ScanlineInfo *scanlines);

void HeavyMystic_StateMischief_Setup(void);
void HeavyMystic_StateBoss_SetupArena(void);
void HeavyMystic_StateBoss_AwaitPlayer(void);
void HeavyMystic_StateMischief_EnterMystic(void);
void HeavyMystic_StateMischief_HandleAppearArc(void);
void HeavyMystic_StateBoss_BeginShow(void);
void HeavyMystic_StateMischief_Idle(void);
void HeavyMystic_StateMischief_MoveIntoPlace(void);
void HeavyMystic_StateMischief_FinishedMagicTrick(void);
void HeavyMystic_StateMischief_PrepareMagicTrick(void);
void HeavyMystic_StateMischief_GoodDay(void);
void HeavyMystic_StateMischief_ConjureHatterkiller(void);
void HeavyMystic_StateMischief_Disappear(void);

void HeavyMystic_StateBoss_Destroyed(void);
void HeavyMystic_StateBoss_Finish(void);
void HeavyMystic_StateBoss_CloseCurtains(void);
void HeavyMystic_StateBoss_AwaitBoxOpened(void);
void HeavyMystic_StateBoss_EnterMystic(void);
void HeavyMystic_StateBoss_GoodDay(void);
void HeavyMystic_StateBoss_BoxCloseDelay(void);
void HeavyMystic_StateBoss_AwaitBoxClosing(void);
void HeavyMystic_StateBoss_AwaitBoxClosed(void);
void HeavyMystic_StateBoss_Transforming(void);
void HeavyMystic_StateBoss_ShowRogue(void);
void HeavyMystic_StateBoss_MysticReveal(void);
void HeavyMystic_StateBoss_MoveToBoxY(void);
void HeavyMystic_StateBoss_MoveToBoxX(void);
void HeavyMystic_StateBoss_TransformBackIntoRogue(void);
void HeavyMystic_StateBoss_FangIdle(void);
void HeavyMystic_StateBoss_FangTell(void);
void HeavyMystic_StateBoss_FangHop(void);
void HeavyMystic_StateBoss_RogueHit(void);
void HeavyMystic_StateBoss_BarkIdle(void);
void HeavyMystic_StateBoss_BarkPounding(void);
void HeavyMystic_StateBoss_BarkJump(void);
void HeavyMystic_StateBoss_BeanIdle(void);
void HeavyMystic_StateBoss_BeanBomb1Throw(void);
void HeavyMystic_StateBoss_BeanBomb2Throw(void);
void HeavyMystic_StateBoss_BeanJump(void);

void HeavyMystic_StateCork_Fired(void);
#if MANIA_USE_PLUS
void HeavyMystic_StateCork_MightyRebound(void);
#endif

void HeavyMystic_State_Bomb(void);

void HeavyMystic_State_BarkDebris(void);

void HeavyMystic_StateBox_AwaitCurtainRise(void);
void HeavyMystic_StateBox_Idle(void);
void HeavyMystic_StateBox_CloseDoors(void);
void HeavyMystic_StateBox_OpenDoors(void);
void HeavyMystic_StateBox_Transforming(void);
void HeavyMystic_StateBox_TransformFinish(void);
void HeavyMystic_StateBox_Dropping(void);
void HeavyMystic_StateBox_ShowContents(void);
void HeavyMystic_StateBox_Reappear(void);

void HeavyMystic_Draw_BoxOpened(void);
void HeavyMystic_Draw_BoxTransforming(void);
void HeavyMystic_Draw_BoxClosed(void);

#endif //! OBJ_HEAVYMYSTIC_H
