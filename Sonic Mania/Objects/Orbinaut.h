#ifndef OBJ_ORBINAUT_H
#define OBJ_ORBINAUT_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectOrbinaut;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityOrbinaut;

// Object Entity
extern ObjectOrbinaut *Orbinaut;

// Standard Entity Events
void Orbinaut_Update();
void Orbinaut_LateUpdate();
void Orbinaut_StaticUpdate();
void Orbinaut_Draw();
void Orbinaut_Create(void* data);
void Orbinaut_StageLoad();
void Orbinaut_EditorDraw();
void Orbinaut_EditorLoad();
void Orbinaut_Serialize();

// Extra Entity Functions


#endif //!OBJ_ORBINAUT_H
