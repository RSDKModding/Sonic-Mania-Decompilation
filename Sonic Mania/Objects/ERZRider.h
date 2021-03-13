#ifndef OBJ_ERZRIDER_H
#define OBJ_ERZRIDER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectERZRider;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityERZRider;

// Object Struct
extern ObjectERZRider *ERZRider;

// Standard Entity Events
void ERZRider_Update();
void ERZRider_LateUpdate();
void ERZRider_StaticUpdate();
void ERZRider_Draw();
void ERZRider_Create(void* data);
void ERZRider_StageLoad();
void ERZRider_EditorDraw();
void ERZRider_EditorLoad();
void ERZRider_Serialize();

// Extra Entity Functions


#endif //!OBJ_ERZRIDER_H
