#ifndef OBJ_GYMBAR_H
#define OBJ_GYMBAR_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectGymBar;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityGymBar;

// Object Struct
extern ObjectGymBar *GymBar;

// Standard Entity Events
void GymBar_Update();
void GymBar_LateUpdate();
void GymBar_StaticUpdate();
void GymBar_Draw();
void GymBar_Create(void* data);
void GymBar_StageLoad();
void GymBar_EditorDraw();
void GymBar_EditorLoad();
void GymBar_Serialize();

// Extra Entity Functions


#endif //!OBJ_GYMBAR_H
