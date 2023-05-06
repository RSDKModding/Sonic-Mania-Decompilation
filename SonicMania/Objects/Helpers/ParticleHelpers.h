#ifndef OBJ_PARTICLEHELPERS_H
#define OBJ_PARTICLEHELPERS_H

#include "Game.h"

// Object Class
struct ObjectParticleHelpers {
    RSDK_OBJECT
};

// Entity Class
struct EntityParticleHelpers {
    RSDK_ENTITY
};

// Object Entity
extern ObjectParticleHelpers *ParticleHelpers;

// Standard Entity Events
void ParticleHelpers_Update(void);
void ParticleHelpers_LateUpdate(void);
void ParticleHelpers_StaticUpdate(void);
void ParticleHelpers_Draw(void);
void ParticleHelpers_Create(void *data);
void ParticleHelpers_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ParticleHelpers_EditorDraw(void);
void ParticleHelpers_EditorLoad(void);
#endif
void ParticleHelpers_Serialize(void);

// Extra Entity Functions
void ParticleHelpers_SetupFallingParticles(int32 x, int32 y, void (*callback)(EntityDebris *));
void ParticleHelpers_SetupParticleFX(void *debrisState, void (*callback)(EntityDebris *), int32 speed, int32 xOffset, int32 yOffset, int32 maxX,
                                     int32 maxY);

#endif //! OBJ_PARTICLEHELPERS_H
