#ifndef OBJ_SHIELD_H
#define OBJ_SHIELD_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
    uint16 sfx_BlueShield;
    uint16 sfx_BubbleShield;
    uint16 sfx_FireShield;
    uint16 sfx_LightningShield;
    uint16 sfx_InstaShield;
    uint16 sfx_BubbleBounce;
    uint16 sfx_FireDash;
    uint16 sfx_LightningJump;
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
