#ifndef OBJ_PARTICLEHELPERS_H
#define OBJ_PARTICLEHELPERS_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectParticleHelpers;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityParticleHelpers;

// Object Entity
extern ObjectParticleHelpers *ParticleHelpers;

// Standard Entity Events
void ParticleHelpers_Update();
void ParticleHelpers_LateUpdate();
void ParticleHelpers_StaticUpdate();
void ParticleHelpers_Draw();
void ParticleHelpers_Create(void* data);
void ParticleHelpers_StageLoad();
void ParticleHelpers_EditorDraw();
void ParticleHelpers_EditorLoad();
void ParticleHelpers_Serialize();

// Extra Entity Functions


#endif //!OBJ_PARTICLEHELPERS_H
