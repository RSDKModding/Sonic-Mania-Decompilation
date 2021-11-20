#ifndef OBJ_SHIELD_H
#define OBJ_SHIELD_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxBlueShield;
    uint16 sfxBubbleShield;
    uint16 sfxFireShield;
    uint16 sfxLightningShield;
    uint16 sfxInstaShield;
    uint16 sfxBubbleBounce;
    uint16 sfxFireDash;
    uint16 sfxLightningJump;
} ObjectShield;

// Entity Class
typedef struct {
    RSDK_ENTITY
    EntityPlayer *player;
    StateMachine(state);
    int32 type;
    int32 timer;
    int32 frameID;
    int32 flag;
    Animator animator;
    Animator animator2;
} EntityShield;

// Object Struct
extern ObjectShield *Shield;

// Standard Entity Events
void Shield_Update(void);
void Shield_LateUpdate(void);
void Shield_StaticUpdate(void);
void Shield_Draw(void);
void Shield_Create(void* data);
void Shield_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Shield_EditorDraw(void);
void Shield_EditorLoad(void);
#endif
void Shield_Serialize(void);

// Extra Entity Functions
void Shield_State_Generic(void);
void Shield_State_Bubble(void);
void Shield_State_BubbleAlt(void);
void Shield_State_Fire(void);
void Shield_State_Lightning(void);
void Shield_State_Insta(void);

#endif //!OBJ_SHIELD_H
