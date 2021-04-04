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
void Pendulum_Update(void);
void Pendulum_LateUpdate(void);
void Pendulum_StaticUpdate(void);
void Pendulum_Draw(void);
void Pendulum_Create(void* data);
void Pendulum_StageLoad(void);
void Pendulum_EditorDraw(void);
void Pendulum_EditorLoad(void);
void Pendulum_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PENDULUM_H
