#ifndef OBJ_TIPPINGPLATFORM_H
#define OBJ_TIPPINGPLATFORM_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTippingPlatform;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateCollide);
    int type;
    Vector2 amplitude;
    int speed;
    int hasTension;
    sbyte frameID;
    byte collision;
    Vector2 targetPos;
    Vector2 centerPos;
    Vector2 drawPos;
    Vector2 collisionOffset;
    int stood;
    int collapseDelay;
    int stoodAngle;
    byte stoodPlayers;
    byte pushPlayersL;
    byte pushPlayersR;
    Hitbox hitbox;
    Animator animator;
    int childCount;
    byte interval;
    byte intervalOffset;
    byte duration;
    byte bossID;
    EntitySyringe *syringe;
} EntityTippingPlatform;

// Object Struct
extern ObjectTippingPlatform *TippingPlatform;

// Standard Entity Events
void TippingPlatform_Update(void);
void TippingPlatform_LateUpdate(void);
void TippingPlatform_StaticUpdate(void);
void TippingPlatform_Draw(void);
void TippingPlatform_Create(void* data);
void TippingPlatform_StageLoad(void);
void TippingPlatform_EditorDraw(void);
void TippingPlatform_EditorLoad(void);
void TippingPlatform_Serialize(void);

// Extra Entity Functions
void TippingPlatform_Unknown1(void);
void TippingPlatform_Unknown2(void);
void TippingPlatform_Unknown3(void);
void TippingPlatform_Unknown4(void);
void TippingPlatform_Unknown5(void);

#endif //!OBJ_TIPPINGPLATFORM_H
