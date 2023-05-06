#ifndef OBJ_LRZCONVCONTROL_H
#define OBJ_LRZCONVCONTROL_H

#include "Game.h"

typedef enum { LRZCONVCTRL_TRIGGER_PLAYER, LRZCONVCTRL_TRIGGER_BUTTON } LRZConvControlTriggerModes;
typedef enum {
    LRZCONVCTRL_BEHAVIOR_ONOFF_SET,
    LRZCONVCTRL_BEHAVIOR_ONOFF_SWAP,
    LRZCONVCTRL_BEHAVIOR_CHANGEDIR_SET,
    LRZCONVCTRL_BEHAVIOR_CHANGEDIR_SWAP
} LRZConvControlBehaviours;
// Object Class
struct ObjectLRZConvControl {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityLRZConvControl {
    RSDK_ENTITY
    StateMachine(state);
    uint8 triggerMode;
    int32 buttonTag;
    Vector2 hitboxSize;
    uint8 behavior;
    uint8 flipVal;
    EntityButton *taggedButton;
    uint8 activePlayers;
    uint8 wasToggled;
    Hitbox hitbox;
    Animator animator;
};

// Object Struct
extern ObjectLRZConvControl *LRZConvControl;

// Standard Entity Events
void LRZConvControl_Update(void);
void LRZConvControl_LateUpdate(void);
void LRZConvControl_StaticUpdate(void);
void LRZConvControl_Draw(void);
void LRZConvControl_Create(void *data);
void LRZConvControl_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void LRZConvControl_EditorDraw(void);
void LRZConvControl_EditorLoad(void);
#endif
void LRZConvControl_Serialize(void);

// Extra Entity Functions
void LRZConvControl_HandlePlayerTrigger(void);
void LRZConvControl_HandleButtonTrigger(void);

#endif //! OBJ_LRZCONVCONTROL_H
