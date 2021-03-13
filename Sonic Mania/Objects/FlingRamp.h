#ifndef OBJ_FLINGRAMP_H
#define OBJ_FLINGRAMP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
} ObjectFlingRamp;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFlingRamp;

// Object Struct
extern ObjectFlingRamp *FlingRamp;

// Standard Entity Events
void FlingRamp_Update();
void FlingRamp_LateUpdate();
void FlingRamp_StaticUpdate();
void FlingRamp_Draw();
void FlingRamp_Create(void* data);
void FlingRamp_StageLoad();
void FlingRamp_EditorDraw();
void FlingRamp_EditorLoad();
void FlingRamp_Serialize();

// Extra Entity Functions


#endif //!OBJ_FLINGRAMP_H
