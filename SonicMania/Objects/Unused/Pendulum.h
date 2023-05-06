#ifndef OBJ_PENDULUM_H
#define OBJ_PENDULUM_H

#include "Game.h"

// Object Class
struct ObjectPendulum {
    RSDK_OBJECT
};

// Entity Class
struct EntityPendulum {
    RSDK_ENTITY
};

// Object Entity
extern ObjectPendulum *Pendulum;

// Standard Entity Events
void Pendulum_Update(void);
void Pendulum_LateUpdate(void);
void Pendulum_StaticUpdate(void);
void Pendulum_Draw(void);
void Pendulum_Create(void *data);
void Pendulum_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Pendulum_EditorDraw(void);
void Pendulum_EditorLoad(void);
#endif
void Pendulum_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_PENDULUM_H
