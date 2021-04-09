#ifndef OBJ_SHIELD_H
#define OBJ_SHIELD_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
    ushort sfx_BlueShield;
    ushort sfx_BubbleShield;
    ushort sfx_FireShield;
    ushort sfx_LightningShield;
    ushort sfx_InstaShield;
    ushort sfx_BubbleBounce;
    ushort sfx_FireDash;
    ushort sfx_LightningJump;
} ObjectShield;

// Entity Class
typedef struct {
    RSDK_ENTITY
    EntityPlayer *player;
    StateMachine(state);
    int type;
    int timer;
    int frameID;
    int flag;
    Animator data;
    Animator altData;
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
void Shield_EditorDraw(void);
void Shield_EditorLoad(void);
void Shield_Serialize(void);

// Extra Entity Functions
void Shield_State_Generic(void);
void Shield_State_Bubble(void);
void Shield_State_BubbleAlt(void);
void Shield_State_Fire(void);
void Shield_State_Lightning(void);
void Shield_State_Insta(void);

#endif //!OBJ_SHIELD_H
