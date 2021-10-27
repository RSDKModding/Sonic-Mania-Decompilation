#ifndef OBJ_ICESPRING_H
#define OBJ_ICESPRING_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
	uint16 animID;
	uint16 bounceSFX;
	uint16 shatterSFX;
	Animator animators[4];
} ObjectIceSpring;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    int32 flipFlag;
    uint8 planeFilter;
    int32 basetimer;
    Animator animator;
    Hitbox hitbox;
	//END SPRING INHERITANCE
    int32 timer;
    uint8 playerBits;
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
#if RETRO_INCLUDE_EDITOR
void IceSpring_EditorDraw(void);
void IceSpring_EditorLoad(void);
#endif
void IceSpring_Serialize(void);

// Extra Entity Functions
void IceSpring_Shatter(int32 velX, int32 velY);

#endif //!OBJ_ICESPRING_H
