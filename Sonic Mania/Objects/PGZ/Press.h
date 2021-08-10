#ifndef OBJ_PRESS_H
#define OBJ_PRESS_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
	ushort animID;
	Hitbox hitbox;
	ushort impactSFX;
	ushort pressSFX;
	bool32 canSuper;
} ObjectPress;

// Entity Class
typedef struct {
	RSDK_ENTITY
	StateMachine(state);
	ushort size;
	uint speed;
	int offTop;
	int offBottom;
	int topOffset;
	uint threads;
	int height;
	int timerStart;
	int timer;
	Vector2 drawPos;
	ushort threadSprY;
	byte onRoof;
	byte onFloor;
	Animator crusherAnimator;
	Animator threadAnimator;
	Animator bumperAnimator;
} EntityPress;

// Object Struct
extern ObjectPress *Press;

// Standard Entity Events
void Press_Update(void);
void Press_LateUpdate(void);
void Press_StaticUpdate(void);
void Press_Draw(void);
void Press_Create(void* data);
void Press_StageLoad(void);
void Press_EditorDraw(void);
void Press_EditorLoad(void);
void Press_Serialize(void);

// Extra Entity Functions
bool32 Press_SuperCheck(int);
void Press_Move(void);
void Press_Crush(void);
void Press_FinalCrush(void);
void Press_HandleMovement(void);
void Press_HandleCrates(void);
void Press_DrawHandle(void);

#endif //!OBJ_PRESS_H
