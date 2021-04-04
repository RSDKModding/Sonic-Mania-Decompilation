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
void Orbinaut_Update(void);
void Orbinaut_LateUpdate(void);
void Orbinaut_StaticUpdate(void);
void Orbinaut_Draw(void);
void Orbinaut_Create(void* data);
void Orbinaut_StageLoad(void);
void Orbinaut_EditorDraw(void);
void Orbinaut_EditorLoad(void);
void Orbinaut_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ORBINAUT_H
