#ifndef OBJ_SCREWMOBILE_H
#define OBJ_SCREWMOBILE_H

#include "Game.h"

typedef enum {
    SCREWMOBILE_MOBILE,
    SCREWMOBILE_BOMB,
} ScrewMobileTypes;

// Object Class
struct ObjectScrewMobile {
    RSDK_OBJECT
    Hitbox hitboxCockpit;
    Hitbox hitboxDepthCharge;
    bool32 playingFanSfx;
    uint8 fanSfxTimer;
    uint16 aniFrames;
    uint16 sfxButton;
    uint16 sfxEggMobile;
    uint16 sfxBigFan;
    uint16 sfxImpact;
};

// Entity Class
struct EntityScrewMobile {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 startPos;
    bool32 bombPress;
    uint8 timer;
    int32 whirlPoolTimer;
    int32 whirlpoolHeight;
    int32 invincibilityTimer;
    uint8 health;
    EntityWhirlpool *whirlpool;
    Animator mobileAnimator;
    Animator propellerAnimator;
    Animator rackAnimator;
    Animator whirlpoolAnimator;
    Animator whirlpoolTopAnimator;
    Animator whirlpoolBottomAnimator;
};

// Object Struct
extern ObjectScrewMobile *ScrewMobile;

// Standard Entity Events
void ScrewMobile_Update(void);
void ScrewMobile_LateUpdate(void);
void ScrewMobile_StaticUpdate(void);
void ScrewMobile_Draw(void);
void ScrewMobile_Create(void *data);
void ScrewMobile_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ScrewMobile_EditorDraw(void);
void ScrewMobile_EditorLoad(void);
#endif
void ScrewMobile_Serialize(void);

// Extra Entity Functions
void ScrewMobile_State_CheckPlayerEnter(void);
void ScrewMobile_State_PlayerRiding(void);
void ScrewMobile_State_BossFinished(void);
void ScrewMobile_State_Idle(void);
void ScrewMobile_Draw_ScrewMobile(void);

void ScrewMobile_StateDepthCharge_Active(void);
void ScrewMobile_StateDepthCharge_Debris(void);
void ScrewMobile_Draw_DepthCharge(void);

#endif //! OBJ_SCREWMOBILE_H
