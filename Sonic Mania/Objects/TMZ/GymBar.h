#ifndef OBJ_GYMBAR_H
#define OBJ_GYMBAR_H

#include "SonicMania.h"

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
void GymBar_Update(void);
void GymBar_LateUpdate(void);
void GymBar_StaticUpdate(void);
void GymBar_Draw(void);
void GymBar_Create(void* data);
void GymBar_StageLoad(void);
void GymBar_EditorDraw(void);
void GymBar_EditorLoad(void);
void GymBar_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_GYMBAR_H
