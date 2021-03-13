#ifndef OBJ_PENDULUM_H
#define OBJ_PENDULUM_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPendulum;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPendulum;

// Object Entity
extern ObjectPendulum *Pendulum;

// Standard Entity Events
void Pendulum_Update();
void Pendulum_LateUpdate();
void Pendulum_StaticUpdate();
void Pendulum_Draw();
void Pendulum_Create(void* data);
void Pendulum_StageLoad();
void Pendulum_EditorDraw();
void Pendulum_EditorLoad();
void Pendulum_Serialize();

// Extra Entity Functions


#endif //!OBJ_PENDULUM_H
