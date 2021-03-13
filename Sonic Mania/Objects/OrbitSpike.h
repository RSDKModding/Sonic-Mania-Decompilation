#ifndef OBJ_ORBITSPIKE_H
#define OBJ_ORBITSPIKE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectOrbitSpike;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityOrbitSpike;

// Object Struct
extern ObjectOrbitSpike *OrbitSpike;

// Standard Entity Events
void OrbitSpike_Update();
void OrbitSpike_LateUpdate();
void OrbitSpike_StaticUpdate();
void OrbitSpike_Draw();
void OrbitSpike_Create(void* data);
void OrbitSpike_StageLoad();
void OrbitSpike_EditorDraw();
void OrbitSpike_EditorLoad();
void OrbitSpike_Serialize();

// Extra Entity Functions


#endif //!OBJ_ORBITSPIKE_H
