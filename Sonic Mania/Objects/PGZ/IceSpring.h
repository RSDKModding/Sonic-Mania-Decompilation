#ifndef OBJ_ICESPRING_H
#define OBJ_ICESPRING_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
	ushort animID;
	ushort bounceSFX;
	ushort shatterSFX;
	Animator animators[4];
} ObjectIceSpring;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int type;
    int flipFlag;
    byte planeFilter;
    int basetimer;
    Animator animator;
    Hitbox hitbox;
	//END SPRING INHERITANCE
    int timer;
    byte playerBits;
} EntityIceSpring;

// Object Struct
extern ObjectIceSpring *IceSpring;

// Standard Entity Events
void IceSpring_Update(void);
void IceSpring_LateUpdate(void);
void IceSpring_StaticUpdate(void);
void IceSpring_Draw(void);
void IceSpring_Create(void* data);
void IceSpring_StageLoad(void);
void IceSpring_EditorDraw(void);
void IceSpring_EditorLoad(void);
void IceSpring_Serialize(void);

// Extra Entity Functions
void IceSpring_Shatter(int velX, int velY);

#endif //!OBJ_ICESPRING_H
