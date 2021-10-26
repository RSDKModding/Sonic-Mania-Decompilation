#ifndef OBJ_PARTICLEHELPERS_H
#define OBJ_PARTICLEHELPERS_H

#include "SonicMania.h"

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
void ParticleHelpers_Update(void);
void ParticleHelpers_LateUpdate(void);
void ParticleHelpers_StaticUpdate(void);
void ParticleHelpers_Draw(void);
void ParticleHelpers_Create(void* data);
void ParticleHelpers_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void ParticleHelpers_EditorDraw(void);
void ParticleHelpers_EditorLoad(void);
#endif
void ParticleHelpers_Serialize(void);

// Extra Entity Functions
void ParticleHelpers_Unknown1(int32 x, int32 y);
void ParticleHelpers_Unknown2(void *debrisState, void (*callback)(EntityDebris *), int32 speed, int32 xOffset, int32 yOffset, int32 maxX, int32 maxY);

#endif //!OBJ_PARTICLEHELPERS_H
